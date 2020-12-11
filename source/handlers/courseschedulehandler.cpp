#include "header/handlers/courseschedulehandler.h"

/*
    * Our structure is like this:
    * [
    * "Saturday" = [ "8:00" = {name: course name, teacher: teacher name, exam: exam data}, "10:00" = {name: "", teacher: "", exam: ""} ... ],
    * "Sunday"   = [ "8:00" = {name: "", teacher: "", exam: ""} ...],
    * ...
    * ]
*/
CourseScheduleHandler::CourseScheduleHandler()
    : is_empty {true}
{
    /*
        * so we just fill our structure (explained above) with empy data's
        * i don't know if there is better way to do this and omit the empty objects.
    */
    QList<QVariant> temp;
    weekly_schedule.reserve(week_days);
    temp.reserve(week_days);
    for (int i{0}; i < week_days; ++i) {
        temp.append(QVariantMap {{QString(QStringLiteral("name")), ""}, {QString(QStringLiteral("exam")), ""}, {QString(QStringLiteral("teacher")), ""}});
    }
    for (int i{0}; i < week_days; ++i) {
        weekly_schedule.append(temp);
    }
}

void CourseScheduleHandler::start(int current)
{
    year = QString::number(current);
    requestTokens();
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
    if (!match.hasMatch()) return false;

    QXmlStreamReader reader(match.captured());
    QMap<QString, QVariant> course_data;
    QString hour, new_exam_format;
    QStringList exam, exam_time;
    QLocale locale {QLocale::Persian, QLocale::Iran};

    if (!reader.readNextStartElement()) return false;
    if (reader.name() != QStringLiteral("Root")) return false;

    while(reader.readNextStartElement()) {
        if(reader.name() != QStringLiteral("row")) continue;
        // the struture is not empty
        this->is_empty = false;

        QXmlStreamAttributes attribute {reader.attributes()};
        course_data["name"] = attribute.value("C2").toString();
        course_data["teacher"] = attribute.value("C4").toString();
        // split exam data to sth like this: ["1399.10.10", "8:30-10:30"]
        exam = attribute.value("C13").toString().split(" ");
        exam_time = exam[1].split("-");
        // I don't know if this is the best solution for converting that format to a localized format
        new_exam_format = locale.toString(QDateTime::fromString(exam[0], QStringLiteral("yyyy.mm.dd")), QStringLiteral("yyyy/mm/dd"))
                // i used exam_time[1] first to show time from left to right
                % QStringLiteral(" ") % locale.toString(QTime::fromString(exam_time[1], QStringLiteral("hh:mm")), QStringLiteral("h:m"))
                % QStringLiteral(" - ") % locale.toString(QTime::fromString(exam_time[0], QStringLiteral("hh:mm")), QStringLiteral("h:m"));
        course_data["exam"] = new_exam_format;

        for (int day_index{0}; day_index < week_days; ++day_index) {
            hour = attribute.value(QStringLiteral("C") + QString::number(day_index + 5)).toString();
            if (hour == "") continue;
            weekly_schedule[day_index][hourIndex(hour)] = course_data;
        }
        reader.skipCurrentElement();
    }
    // clear the structure to save the memory
    if (is_empty) weekly_schedule.clear();
    // emit the signal to check wether is_empty is set or not in qml.
    emit isEmptyChanged();
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

QList<QVariant> CourseScheduleHandler::dailyScheduleModel(int day) const
{
    if (is_empty) return QList<QVariant>();
    return weekly_schedule[day];
}

int CourseScheduleHandler::hourIndex(QString& hour) const
{
    // hour is something like this: "08:00-10:00"
    const QStringList hours{QStringLiteral("08"), QStringLiteral("10"), QStringLiteral("13"), QStringLiteral("15"), QStringLiteral("17")};
    for (int i{0}; i < hour.size(); ++i) {
        if (hour.startsWith(hours[i])) {
            return i;
        }
    }
    return -1;
}


