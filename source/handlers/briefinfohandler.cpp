#include "header/handlers/briefinfohandler.h"

BriefInfoHandler::BriefInfoHandler() : locale{QLocale::Persian, QLocale::Iran}, current_year{0}
{
    // we don't need to numbers being separated by thousands
    locale.setNumberOptions(QLocale::OmitGroupSeparator);
}

int BriefInfoHandler::getCurrentYear() const
{
    return current_year;
}

QVariantMap BriefInfoHandler::getStudentInfo() const
{
    QVariantMap data {student_info};
    data[info_title[Index_Id]] = locale.toString(data[info_title[Index_Id]].toULongLong());
    data[info_title[Index_Passed]] = locale.toString(static_cast<int>(data[info_title[Index_Passed]].toFloat()));
    data[info_title[Index_TotalAvg]] = locale.toString(data[info_title[Index_TotalAvg]].toFloat());
    return data;
}

void BriefInfoHandler::start()
{
    requestTokens();
}

QStringList BriefInfoHandler::getSemesterAvgs() const
{
    return passed_semesters_avg;
}

QStringList BriefInfoHandler::getSemesterYears() const
{
    QStringList list;
    for (int year : passed_semesters) {
        // years are like this: 3991. we just need the 991 part.
        list << locale.toString(year % 1000);
    }
    return list;
}

bool BriefInfoHandler::requestTokens()
{
    connect(&request, &Network::complete, this, &BriefInfoHandler::parseTokens);
    QString tck_token {cookies.contains("ctck") ? cookies.value("ctck") : request_validators.value("tck")};
    request.setUrl(root_url + user_info_url + tck_token);
    request.addHeader("Cookie", getCookies().toUtf8());
    return request.get();
}

void BriefInfoHandler::parseTokens(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &BriefInfoHandler::parseTokens);
    QString data;
    if (!verifyResponse(reply, data)) return;

    request_validators.insert(extractFormValidators(data));

    requestStuId();
}

bool BriefInfoHandler::requestStuId()
{
    connect(&request, &Network::complete, this, &BriefInfoHandler::parseStuId);
    request.setUrl(root_url + user_info_url + request_validators["tck"]);
    request.addHeader("Content-Type", "application/x-www-form-urlencoded");
    request.addHeader("Cookie", getCookies().toUtf8());

    QString ticket_tbox { cookies.contains("ctck") ? cookies["ctck"] : request_validators["tck"]};
    QString data{QStringLiteral("__VIEWSTATE=")             % QUrl::toPercentEncoding(request_validators["__VIEWSTATE"])
                % QStringLiteral("&__VIEWSTATEGENERATOR=")  % request_validators["__VIEWSTATEGENERATOR"]
                % QStringLiteral("&__EVENTVALIDATION=")     % QUrl::toPercentEncoding(request_validators["__EVENTVALIDATION"])
                // TicketTextBox should be equal to "ctck" when both "ctck" and "tck" are available
                % QStringLiteral("&TicketTextBox=")         % ticket_tbox
                % QStringLiteral("&Fm_Action=00&Frm_Type=&Frm_No=&XMLStdHlp=&TxtMiddle=%3Cr%2F%3E&ex=")};

    return request.post(data.toUtf8());
}

void BriefInfoHandler::parseStuId(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &BriefInfoHandler::parseStuId);

    QString data, user_number;
    if (!verifyResponse(reply, data)) return;

    request_validators.insert(extractFormValidators(data));
    user_number = extractStuId(data);
    if (user_number == QString()) {
        setErrorCode(Errors::ExtractError);
        reply.deleteLater();
        setSuccess(false);
        setFinished(true);
        return;
    }
    student_info["id"] = user_number;
    requestBriefInfo();
}

bool BriefInfoHandler::requestBriefInfo()
{
    connect(&request, &Network::complete, this, &BriefInfoHandler::parseUserInfo);
    request.setUrl(root_url + user_info_url + request_validators["tck"]);
    request.addHeader("Content-Type", "application/x-www-form-urlencoded");
    request.addHeader("Cookie", getCookies().toUtf8());

    QString ticket_tbox { cookies.contains("ctck") ? cookies["ctck"] : request_validators["tck"]};
    QString data{QStringLiteral("__VIEWSTATE=")                  % QUrl::toPercentEncoding(request_validators["__VIEWSTATE"])
                % QStringLiteral("&__VIEWSTATEGENERATOR=")       % request_validators["__VIEWSTATEGENERATOR"]
                % QStringLiteral("&__EVENTVALIDATION=")          % QUrl::toPercentEncoding(request_validators["__EVENTVALIDATION"])
                % QStringLiteral("&TicketTextBox=")              % ticket_tbox
                % QStringLiteral("&TxtMiddle=%3Cr+F41251%3D%22") % student_info["id"].toString()
                % QStringLiteral("%22%2F%3E&Fm_Action=08&Frm_Type=&Frm_No=&XMLStdHlp=&ex=")};

    return request.post(data.toUtf8());
}

void BriefInfoHandler::parseUserInfo(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &BriefInfoHandler::parseUserInfo);
    QString data;
    if (!verifyResponse(reply, data)) return;

    if (!extractStudentInfo(data) || !extractStudentAvgs(data)) {
        setErrorCode(Errors::ExtractError);
        setSuccess(false);
        setFinished(true);
        return;
    }
//    qDebug() << student_info;
//    qDebug() << student_avgs;
    emit studentInfoChanged();
    setSuccess(true);
    setFinished(true);
    return;
}

bool BriefInfoHandler::extractStudentInfo(const QString& response)
{
    const QList<QString> keywords {QStringLiteral("F17551"),        // Field
                                   QStringLiteral("F41351"),        // StudyType
                                   QStringLiteral("F41701"),        // TotalAvg
                                   QStringLiteral("F41801")};      // Passed
    int position;
    QString value;
    /* increased Index_START by 2 because we want to skip the Id since we don't have Id in this data.
      (we did that in extractStuId) */
    for (int title_index{Index_START + 2}, keyindex{0}; title_index < Index_END; ++title_index, ++keyindex) {
        position = response.indexOf(keywords[keyindex]);
        if (position == -1) return false;

        // 10 is the lentgh of keyword which for example is "F51851 = '"
        // so we need to skip this
        position += 10;
        for (int i{position}; response[i] != "'"; ++i) {
            value.append(response[i]);
        }
        student_info[info_title[title_index]] = value;
        value.clear();
    }
    return true;
}

bool BriefInfoHandler::extractStudentAvgs(const QString &response)
{
    int year_position, avg_position;
    QString year_value, avg_value;
    const QString year_keyword {QStringLiteral("F4350")}, avg_keyword {QStringLiteral("F4360")};
    year_position = response.indexOf(year_keyword);
    avg_position = response.indexOf(avg_keyword);
    if (year_position == -1 || avg_position == -1) {
        return false;
    }

    while (year_position != -1 && avg_position != -1) {
        // 7 is the lentgh of actual keywords which for example is 'F4350="'
        // so we need to skip these characters
        year_position += 7;
        avg_position += 7;
        for (int i{year_position}; response[i] != '"'; ++i) {
            year_value.append(response[i]);
        }

        for (int i{avg_position}; response[i] != '"'; ++i) {
            avg_value.append(response[i]);
        }
        passed_semesters.append(year_value.toInt());
        passed_semesters_avg.append(avg_value);

        year_value.clear();
        avg_value.clear();
        year_position = response.indexOf(year_keyword, year_position);
        avg_position = response.indexOf(avg_keyword, avg_position);
    }
    current_year = passed_semesters.last();

    if (passed_semesters_avg.last().isEmpty()) {
        passed_semesters_avg.pop_back();
        passed_semesters.pop_back();
    }
    emit currentYearChanged();
    return true;
}

QString BriefInfoHandler::extractStuId(const QString &response)
{
    int position {response.indexOf(QStringLiteral("=&quot;"))};
    QString stu_number;
    if (position == -1) return QString(); // return error
    // 7 is the lentgh of string we searched. we need to skip this string.
    int char_position {position + 7};
    while (response[char_position] != '&') {
        stu_number.append(response[char_position]);
        ++char_position;
    }
    return stu_number;
}
