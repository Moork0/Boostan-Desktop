#include "header/handlers/courseschedulehandler.h"

CourseScheduleHandler::CourseScheduleHandler()
{
}

void CourseScheduleHandler::start(const QString current_semester)
{
    setSemester(current_semester);

    requestTokens();
}

QVariantList CourseScheduleHandler::getSchedule() const
{
    return weekly_schedule;
}

bool CourseScheduleHandler::requestTokens()
{
    connect(&request, &Network::complete, this, &CourseScheduleHandler::parseTokens);
    QString tck_token {getTckToken()};
    request.setUrl(root_url + schedule_url + tck_token);
    request.addHeader("Cookie", getCookies().toUtf8());
    return request.get();
}

void CourseScheduleHandler::parseTokens(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &CourseScheduleHandler::parseTokens);
    QString data;
    if (!verifyResponse(reply, data)) {
        reply.deleteLater();
        setSuccess(false);
        setFinished(true);
        return;
    }

    reply.deleteLater();

    request_validators.insert(extractFormValidators(data));
    requestSchedule();
}

bool CourseScheduleHandler::requestSchedule()
{
    connect(&request, &Network::complete, this, &CourseScheduleHandler::parseSchedule);
    QString tck_token {getTckToken()};
    request.setUrl(root_url + schedule_url + tck_token);
    request.addHeader("Cookie", getCookies().toUtf8());
    request.addHeader("Content-Type", "application/x-www-form-urlencoded");

    QString data{QStringLiteral("__VIEWSTATE=")             % QUrl::toPercentEncoding(request_validators["__VIEWSTATE"])
                % QStringLiteral("&__VIEWSTATEGENERATOR=")  % request_validators["__VIEWSTATEGENERATOR"]
                % QStringLiteral("&__EVENTVALIDATION=")     % QUrl::toPercentEncoding(request_validators["__EVENTVALIDATION"])
                % QStringLiteral("&TicketTextBox=")         % tck_token

                // below is like this: <Root><N+UQID="10"+id="1"+F="%1"+T="%1"/></Root> in url encoded format
                % QStringLiteral("&XmlPriPrm=")             % QString(QStringLiteral("%3CRoot%3E%3CN+UQID%3D%2210%22+id%3D%221%22+F%3D%22%1%22+T%3D%22%1%22%2F%3E%3C%2FRoot%3E")).arg(semester)
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
    bool parse_success {true};

    if (!verifyResponse(reply, data))
        parse_success = false;

    request_validators.insert(extractFormValidators(data));
    if (parse_success && !extractWeeklySchedule(data)) {
        setErrorCode(Errors::ExtractError);
        parse_success = false;
    }

    reply.deleteLater();
    if (!parse_success) {
        setSuccess(false);
        setFinished(true);
        return;
    }
    setSuccess(true);
    setFinished(true);
}

void CourseScheduleHandler::setSemester(const QString &sem)
{
    semester = sem;
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

        QXmlStreamAttributes attribute {reader.attributes()};
        map[QStringLiteral("teacher")] = attribute.value(QStringLiteral("C7")).toString();
        map[QStringLiteral("name")] = attribute.value(QStringLiteral("C2")).toString();

        temp_stringlist = attribute.value(QStringLiteral("C1")).toString().split(QStringLiteral("_"));
        // Generate unique id using ScheduleTable::getUid
        map[QStringLiteral("uid")] = ScheduleTable::getUid(temp_stringlist.at(0), temp_stringlist.at(1));

        temp_stringlist = attribute.value("C8").toString().replace(QStringLiteral("ك"), QStringLiteral("ک")).simplified().split("،");
        int counter {0}, exam_index {-1};
        // clear data's for storing new informations
        rows.clear();
        columns.clear();
        lengths.clear();

        for (QString& daytime_str : temp_stringlist) {
            daytime_str = daytime_str.simplified();

            // find the index of exam time in temp_stringlist
            if (daytime_str.startsWith(QStringLiteral("امتحان"))) {
                exam_index = counter;
                break;
            }

            int time_index {daytime_str.indexOf('-') - 5};
            int day_index {daytime_str.indexOf(':') + 2};
            // storing hour string: 12:34-56:78
            temp_string = daytime_str.mid(time_index, 11);
            float calculated_column {ScheduleTable::calculateScheduleColumn(temp_string)};

            columns.append(calculated_column);
            rows.append(ScheduleTable::calculateScheduleRow(daytime_str.mid(day_index, time_index - 1 - day_index)));
            lengths.append(ScheduleTable::calculateScheduleLen(temp_string, calculated_column));

            ++counter;
        }

        exam_string.clear();
        // if we have exam time specified, Iterate over them.
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

    // check if the response data was empty or not
    if (!map.isEmpty())
        setIsEmpty(false);

    return true;
}
