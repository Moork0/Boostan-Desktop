#include "header/handlers/courseschedulehandler.h"

CourseScheduleHandler::CourseScheduleHandler()
{
}

void CourseScheduleHandler::start()
{
    QDir::setCurrent("/home/moorko/cpp/boostan/boostan/test/");
    QFile file("res2.html");
    if (file.open(QIODevice::ReadOnly)) {
        QString rr {file.readAll()};
        extractWeeklySchedule(rr);
    } else {
        qDebug() << file.errorString();
    }
    setSuccess(true);
    setFinished(true);
//    requestTokens();
}

QVariantList CourseScheduleHandler::getSchedule() const
{
    return weekly_schedule;
}

bool CourseScheduleHandler::requestTokens()
{
    connect(&request, &Network::complete, this, &CourseScheduleHandler::parseTokens);
    request.setUrl(root_url + schedule_url + request_validators["tck"]);
    request.addHeader("Cookie", getCookies().toUtf8());
    return request.get();
}

void CourseScheduleHandler::parseTokens(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &CourseScheduleHandler::parseTokens);
    QString data;
    if (!verifyResponse(reply, data)) return;

    request_validators.insert(extractFormValidators(data));
    requestSchedule();
}

bool CourseScheduleHandler::requestSchedule()
{
    connect(&request, &Network::complete, this, &CourseScheduleHandler::parseSchedule);
    request.setUrl(root_url + schedule_url + request_validators["tck"]);
    request.addHeader("Cookie", getCookies().toUtf8());
    request.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // determine the ticketbox value
    QString ticket_tbox { cookies.contains("ctck") ? cookies["ctck"] : request_validators["tck"]};
    QString data{QStringLiteral("__VIEWSTATE=")             % QUrl::toPercentEncoding(request_validators["__VIEWSTATE"])
                % QStringLiteral("&__VIEWSTATEGENERATOR=")  % request_validators["__VIEWSTATEGENERATOR"]
                % QStringLiteral("&__EVENTVALIDATION=")     % QUrl::toPercentEncoding(request_validators["__EVENTVALIDATION"])
                % QStringLiteral("&TicketTextBox=")         % ticket_tbox

                // below is like this: <Root><N+UQID="15"+id="4"+F="%1"+T="%1"/></Root> in url encoded format
                % QStringLiteral("&XmlPriPrm=")             % QString(QStringLiteral("%3CRoot%3E%3CN+UQID%3D%2215%22+id%3D%224%22+F%3D%22%1%22+T%3D%22%1%22%2F%3E%3C%2FRoot%3E")).arg(year)
                % QStringLiteral("&Fm_Action=09&Frm_Type=&Frm_No=&F_ID=&XmlPubPrm=&XmlMoredi=&F9999=&HelpCode=&Ref1=&Ref2=&Ref3=&Ref4=&Ref5=&NameH=&FacNoH=&GrpNoH=&RepSrc=&ShowError=&TxtMiddle=%3Cr%2F%3E&tbExcel=&txtuqid=&ex=")};
    return request.post(data.toUtf8());
}

bool CourseScheduleHandler::getIsEmpty () const
{
    return is_empty;
}

void CourseScheduleHandler::parseSchedule(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &CourseScheduleHandler::parseSchedule);
    QString data;
    if (!verifyResponse(reply, data)) return;
    request_validators.insert(extractFormValidators(data));
    if (!extractWeeklySchedule(data)) {
        setErrorCode(Constants::Errors::ExtractError);
        setSuccess(false);
        setFinished(true);
        reply.deleteLater();
        return;
    }
    setSuccess(true);
    setFinished(true);
    reply.deleteLater();
}

bool CourseScheduleHandler::extractWeeklySchedule(QString& response)
{
    QRegularExpression re {xmldata_pattern, QRegularExpression::UseUnicodePropertiesOption};
    QRegularExpressionMatch match {re.match(response)};
    QVariantMap map;
    if (!match.hasMatch()) return false;

    QXmlStreamReader reader(match.captured());

    if (!reader.readNextStartElement()) return false;
    if (reader.name() != QStringLiteral("Root")) return false;

    QString temp_string, exam_string;
    QStringList temp_stringlist;
    QVariantList rows, columns, lengths;
    while(reader.readNextStartElement()) {
        if(reader.name() != QStringLiteral("row")) continue;
        // the struture is not empty
        setIsEmpty(false);

        QXmlStreamAttributes attribute {reader.attributes()};
        map[QStringLiteral("teacher")] = attribute.value("C7").toString();
        map[QStringLiteral("name")] = attribute.value("C2").toString();

        temp_stringlist = attribute.value("C1").toString().split("_");
        map[QStringLiteral("uid")] = ScheduleTable::getUid(temp_stringlist.at(0), temp_stringlist.at(1));

        temp_stringlist = attribute.value("C8").toString().replace(QStringLiteral("ك"), QStringLiteral("ک")).simplified().split("،");
        int counter {0}, exam_index {-1};
        rows.clear();
        columns.clear();
        lengths.clear();
        for (QString& daytime_str : temp_stringlist) {
            daytime_str = daytime_str.simplified();
            if (daytime_str.startsWith("امتحان")) {
                exam_index = counter;
                break;
            }

            int time_index {daytime_str.indexOf("-") - 5};
            int day_index {daytime_str.indexOf(":") + 2};
            // storing hour string: 12:34-56:78
            temp_string = daytime_str.mid(time_index, 11);
            float calculated_column {calculateScheduleColumn(temp_string)};

            columns.append(calculated_column);
            rows.append(calculateScheduleRow(daytime_str.mid(day_index, time_index - 1 - day_index)));
            lengths.append(calculateScheduleLen(temp_string, calculated_column));

            ++counter;
        }

        exam_string.clear();
        if (exam_index != -1) {
            for (; exam_index < temp_stringlist.size(); ++exam_index) {
                temp_string = temp_stringlist.at(exam_index);
                exam_string += temp_string.midRef(7, 10) % QStringLiteral(" ") % temp_string.rightRef(11) % QStringLiteral(" || ");
            }

        } else {
            exam_string = QStringLiteral("نامشخص || ");
        }
        // remove " || "  from end of the string
        exam_string.chop(4);

        map[QStringLiteral("row")] = rows;
        map[QStringLiteral("column")] = columns;
        map[QStringLiteral("length")] = lengths;
        map[QStringLiteral("exam")] = exam_string;

        weekly_schedule.append(map);
        reader.skipCurrentElement();
    }

    return true;
}

bool CourseScheduleHandler::extractCurrentYear(QString& response)
{
    int position {response.indexOf(QStringLiteral("f=\"3"))};
    if (position == -1) return false;
    year.clear();
    // we should skip 3 characters
    position += 3;
    while (response[position] != '"') {
        year.append(response[position]);
        ++position;
    }
    return true;
}

int CourseScheduleHandler::calculateScheduleRow(const QString& day) const
{
    static const QStringList days_keyword{ QStringLiteral("شنبه"), QStringLiteral("يک"), QStringLiteral("دو"), QStringLiteral("سه"), QStringLiteral("چهار"), QStringLiteral("پنج"), QStringLiteral("جمعه") };
    static const int keyword_size {days_keyword.size()};
    for (int i {0}; i < keyword_size; ++i) {
        if (day.startsWith(days_keyword[i])) {
            return i;
        }
    }
    return -1;
}

float CourseScheduleHandler::calculateScheduleColumn(const QString& hour) const
{
    constexpr int first_hour {8};
    constexpr int last_hour {20};
    constexpr int columns_length {last_hour - first_hour};
    QString current_hour;
    for (int i {0}; i < columns_length; ++i) {
        current_hour = QString::number(first_hour + i);
        if (current_hour.size() == 1)
            current_hour = QString(QStringLiteral("0")) + current_hour;

        if (hour.startsWith(current_hour))
            return i + (hour.midRef(3, 2).toFloat() / 60);
    }
    return -1;
}

float CourseScheduleHandler::calculateScheduleLen(const QString& hour, const float start_column) const
{
    // 5 is the length of the last 5 character of "12:34-56:78" which is "56:78"
    float end_column {calculateScheduleColumn(hour.right(5))};
    return end_column - start_column;
}
