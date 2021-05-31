#include "header/handlers/offeredcoursehandler.h"

// Retrieve and deserialize the possible information of Schedule table
OfferedCourseHandler::OfferedCourseHandler()
    :   _schedule (ScheduleTable::deserialize(Settings::getValue(QStringLiteral("offeredSchedule")).toString())),
        _request_number{0}
{

}

OfferedCourseHandler::~OfferedCourseHandler()
{
    cleanUp();
}

bool OfferedCourseHandler::getIsEmpty() const
{
    return _is_empty;
}

void OfferedCourseHandler::cleanUp()
{
    if (_container.isEmpty()) return;
    for (QVariantList* element : qAsConst(_container)) {
        delete element;
        element = nullptr;
    }
    _container.clear();
}

void OfferedCourseHandler::normalizeTime(QString &time)
{
    time.replace(MyStringLiteral("ك"), MyStringLiteral("ک"));
    time = time.simplified();
}

bool OfferedCourseHandler::CheckIsChoosed(const QString& key, const QHash<QString, QVariantMap> &schedule) const
{
    return schedule.contains(key);
}

void OfferedCourseHandler::start()
{
    requestCourses();
}

void OfferedCourseHandler::requestCourses()
{
    connect(&request, &Network::complete, this, &OfferedCourseHandler::parseCourses);
    const QString tck_token {getTckToken()};
    const QString url {_offered_course_url.arg(_url_fids.at(_request_number))};
    request.setUrl(_root_url + url + tck_token);
    request.addHeader("Cookie", getCookies().toUtf8());
    request.get();
}

void OfferedCourseHandler::parseCourses(QNetworkReply &reply)
{
    disconnect(&request, &Network::complete, this, &OfferedCourseHandler::parseCourses);
    QString data;
    bool parse_success {true};
    if (!verifyResponse(reply, data))
        parse_success = false;

    if (parse_success && !extractOfferedCourses(data)) {
        setErrorCode(Errors::ExtractError);
        parse_success = false;
    }

    reply.deleteLater();

    if (!parse_success) {
        ++_request_number;
        if (_request_number < (_url_fids.size())) {
            requestCourses();
            return;
        }
        setSuccess(false);
        setFinished(true);
        return;
    }
    setSuccess(true);
    setFinished(true);
}

bool OfferedCourseHandler::extractOfferedCourses(const QString& response)
{
    QRegularExpression re {_xmldata_pattern, QRegularExpression::UseUnicodePropertiesOption};
    QRegularExpressionMatch match {re.match(response)};
    if (!match.hasMatch()) return false;

    QXmlStreamReader reader(match.captured());
    /* REGEX patterns for extracting exam date and time */
    QRegularExpression re_exam_date {QStringLiteral("\\.(\\d{2}.\\d{2})"), QRegularExpression::UseUnicodePropertiesOption};
    QRegularExpression re_exam_time {QStringLiteral("(\\d{2}:\\d{2})-\\d{2}:\\d{2}"), QRegularExpression::UseUnicodePropertiesOption};

    QVariantList* row_datas;

    QStringRef temp_data_ref;
    QString temp_data;
    QString course_uid;
    QStringList splited_data;
    int column_count = OfferedCourseModel::columns.count();
    if (!reader.readNextStartElement()) return false;
    if (reader.name() != QStringLiteral("Root")) return false;

    while(reader.readNextStartElement()) {
        if(reader.name() != QStringLiteral("row")) continue;

         QXmlStreamAttributes attribute {reader.attributes()};

         row_datas = new QVariantList;
        row_datas->reserve(column_count);
        for (int i {0}; i < column_count; ++i) row_datas->append(QVariant());
        /*
         * the order of the values which pushed into the row_data is important
         * because the order should be equal to the order of roleNames in the OfferedCourseModel
         * otherwise the information would be incorrect
         */
        temp_data = attribute.value("C1").toString();

        // splitters for each page is different
        switch (_request_number) {
        case 0:
            splited_data = temp_data.split("_");
            break;
        case 1:
            splited_data = temp_data.split("-");
            break;

        default:
            return false;
        }

        course_uid = ScheduleTable::getUid(splited_data.at(0), splited_data.at(1));
        // course number
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::courseNumberRole), splited_data[0]);
        // group
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::groupRole), splited_data[1]);
        // course name
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::courseNameRole),
                           attribute.value("C2").toString());
        // weight
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::weightRole),
                           attribute.value("C3").toInt());
        // capacity
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::capacityRole),
                           attribute.value("C5").toInt());

        temp_data_ref = attribute.value("C6");

        // sex
        if (temp_data_ref == MyStringLiteral("زن"))
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::sexRole), OfferedCourseModel::Female);

        else if (temp_data_ref == MyStringLiteral("مرد"))
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::sexRole), OfferedCourseModel::Male);

        else
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::sexRole), OfferedCourseModel::None);

        // teacher
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::teacherRole),
                           attribute.value("C7").toString().remove(QStringLiteral("<BR>")));

        // time and exam
        temp_data = attribute.value("C8").toString();
        temp_data.remove(MyStringLiteral("درس(ت): "));
        temp_data.remove(MyStringLiteral("درس(ع): "));
        /// TODO: determine theory and practical courses

        /*
         * This part of information was not clear for me. So i developed a more generic solution for extracting
         * time and exam date/time from the text.
         * The approximate form of the splited data would be like this:
         * [course time1, course time 2,..., Exam data1(could be one or more exam), Exam data2((could be one or more exam))]
         * Exam data's could be not available.
         */
        splited_data = temp_data.split(QStringLiteral("<BR>"));
        temp_data.clear();
        splited_data.pop_back();
        // First index of the exam information occurance
        int exam_index {-1};
        // number of course times
        int class_time_number {splited_data.size()};
        for (int i {0}; i < class_time_number; ++i) {
            // find the first occurance of exam informations
            if (splited_data.at(i).contains(MyStringLiteral("امتحان"))) {
                exam_index = i;
                class_time_number = i;
                break;
            }
        }

        for (int i {0}; i < class_time_number; ++i) {
            temp_data += splited_data[i] + QStringLiteral("<br>");
        }
        // remove the '<br>' from end of the string
        temp_data.chop(4);
        normalizeTime(temp_data);
        // time
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::timeRole), temp_data);

        temp_data = MyStringLiteral("نامشخص");
        if (exam_index != -1) {
            temp_data.clear();
            // iterate over exam informations and extract the data's by using REGEX
            for (; exam_index < splited_data.size(); ++exam_index) {
               QRegularExpressionMatchIterator date_match = re_exam_date.globalMatch(splited_data[exam_index]);
               QRegularExpressionMatchIterator time_match = re_exam_time.globalMatch(splited_data[exam_index]);
               while (date_match.hasNext()) {
                   // template: month.day/hour:minute
                   /// TODO: change template to a more-readable form
                   temp_data += date_match.next().captured(1) % QStringLiteral("/") % time_match.next().captured(1) % QStringLiteral("<br>");
               }
            }
            // remove the '<br>' from end of the string
            temp_data.chop(4);
        }
        // exam
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::examRole), temp_data);

        // place
        temp_data_ref = attribute.value("C9");
        int underline_pos {temp_data_ref.lastIndexOf('_')};
        int endpos {temp_data_ref.lastIndexOf("<BR>")};
        if (underline_pos != -1 && endpos != -1)
            // we skip 2 character which is '_ '
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::placeRole),
                               temp_data_ref.mid(underline_pos + 2, endpos - underline_pos - 2).toString());
        else
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::placeRole),
                               temp_data_ref.toString());

        // isChoosed
        if (CheckIsChoosed(course_uid, _schedule)) {
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::isChoosedRole), true);
            _container.push_front(row_datas);
        } else {
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::isChoosedRole), false);
            _container.push_back(row_datas);
        }

        reader.skipCurrentElement();
    }

    // check if the struture is not empty
    if (!_container.isEmpty())
        setIsEmpty(false);

    return true;
}

void OfferedCourseHandler::sendDataTo(QObject* model)
{
    OfferedCourseModel* view_model = reinterpret_cast<OfferedCourseModel*>(model);
    view_model->setDataContainer(_container);
}

// convert 'schedule' to a container format which the ScheduleTable could parse
QVariantList OfferedCourseHandler::restoreSchedule() const
{
    if (_schedule.isEmpty()) return QVariantList();

    QVariantList schedule_list;
    QHash<QString, QVariantMap>::const_iterator iterator = _schedule.cbegin();
    QHash<QString, QVariantMap>::const_iterator end = _schedule.cend();
    for (; iterator != end; ++iterator) {
        schedule_list.push_back(iterator.value());
    }
    return schedule_list;
}
