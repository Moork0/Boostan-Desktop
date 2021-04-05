#include "header/handlers/offeredcoursehandler.h"

// Retrieve and deserialize the possible information of Schedule table
OfferedCourseHandler::OfferedCourseHandler()
    :   schedule (ScheduleTable::deserialize(Settings::getValue(QStringLiteral("offeredSchedule")).toString())),
        request_number{0}
{

}

OfferedCourseHandler::~OfferedCourseHandler()
{
    cleanUp();
}

bool OfferedCourseHandler::getIsEmpty() const
{
    return is_empty;
}

void OfferedCourseHandler::cleanUp()
{
    if (container.isEmpty()) return;
    for (QVariantList* element : qAsConst(container)) {
        delete element;
        element = nullptr;
    }
    container.clear();
}

void OfferedCourseHandler::normalizeTime(QString &time)
{
    time.replace(QStringLiteral("ك"), QStringLiteral("ک"));
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
    const QString url {offered_course_url.arg(url_fids.at(request_number))};
    request.setUrl(root_url + url + tck_token);
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
        ++request_number;
        if (request_number < (url_fids.size())) {
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
    QRegularExpression re {xmldata_pattern, QRegularExpression::UseUnicodePropertiesOption};
    QRegularExpressionMatch match {re.match(response)};
    if (!match.hasMatch()) return false;

    QXmlStreamReader reader(match.captured());
    /* REGEX patterns for extracting exam date/time */
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
        switch (request_number) {
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
        if (temp_data_ref == QStringLiteral("زن"))
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::sexRole), OfferedCourseModel::Female);

        else if (temp_data_ref == QStringLiteral("مرد"))
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::sexRole), OfferedCourseModel::Male);

        else
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::sexRole), OfferedCourseModel::None);

        // teacher
        row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::teacherRole),
                           attribute.value("C7").toString().remove(QStringLiteral("<BR>")));

        // time and exam
        temp_data = attribute.value("C8").toString();
        temp_data.remove(QStringLiteral("درس(ت): "));
        temp_data.remove(QStringLiteral("درس(ع): "));
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
            if (splited_data.at(i).contains(QStringLiteral("امتحان"))) {
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

        temp_data = QStringLiteral("نامشخص");
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
        if (CheckIsChoosed(course_uid, schedule)) {
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::isChoosedRole), true);
            container.push_front(row_datas);
        } else {
            row_datas->replace(OfferedCourseModel::roleToIndex(OfferedCourseModel::isChoosedRole), false);
            container.push_back(row_datas);
        }

        reader.skipCurrentElement();
    }

    // check if the struture is not empty
    if (!container.isEmpty())
        setIsEmpty(false);

    return true;
}

void OfferedCourseHandler::sendDataTo(QObject* model)
{
    OfferedCourseModel* view_model = reinterpret_cast<OfferedCourseModel*>(model);
    view_model->setDataContainer(container);
}

// convert 'schedule' to a container format which the ScheduleTable could parse
QVariantList OfferedCourseHandler::restoreSchedule() const
{
    if (schedule.isEmpty()) return QVariantList();

    QVariantList schedule_list;
    QHash<QString, QVariantMap>::const_iterator iterator = schedule.cbegin();
    QHash<QString, QVariantMap>::const_iterator end = schedule.cend();
    for (; iterator != end; ++iterator) {
        schedule_list.push_back(iterator.value());
    }
    return schedule_list;
}
