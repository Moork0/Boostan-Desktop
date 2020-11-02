#include "header/dashboardhandler.h"

BriefInfoHandler::BriefInfoHandler() : locale{QLocale::Persian, QLocale::Iran}
{
    locale.setNumberOptions(QLocale::OmitGroupSeparator);
}

QVariantMap BriefInfoHandler::getStudentInfo() const
{
    QVariantMap data;
    for (auto it {student_info.cbegin()}; it != student_info.cend(); ++it) {
        data[it.key()] = it.value();
    }
    data[info_title[INDEX_Id]] = locale.toString(data[info_title[INDEX_Id]].toULongLong());
    data[info_title[INDEX_Passed]] = locale.toString(static_cast<int>(data[info_title[INDEX_Passed]].toFloat()));
    data[info_title[INDEX_TotalAvg]] = locale.toString(data[info_title[INDEX_TotalAvg]].toFloat());
    return data;
}

void BriefInfoHandler::start()
{
    requestTokens();
}

QStringList BriefInfoHandler::getSemesterAvgs() const
{
    QStringList list;
    for (float average : passed_semesters_avg) {
        list << locale.toString(average);
    }
    return list;
}

QStringList BriefInfoHandler::getSemesterYears() const
{
    QStringList list;
    for (int year : passed_semesters) {
        list << locale.toString(year % 1000);
    }
    return list;
}

bool BriefInfoHandler::requestTokens()
{
    connect(&request, &Network::complete, this, &BriefInfoHandler::parseTokens);
    request.setUrl(root_url + user_info_url + request_validators["tck"]);
    request.addHeader("Cookie", getCookies().toUtf8());
    return request.get();
}

void BriefInfoHandler::parseTokens(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &BriefInfoHandler::parseTokens);
    QString data;
    if (!verifyResponse(reply, data)) return;

    request_validators.insert(TextParser::Validators::extractFormValidators(data));

    requestStuId();
}

bool BriefInfoHandler::requestStuId()
{
    connect(&request, &Network::complete, this, &BriefInfoHandler::parseUserNumber);
    request.setUrl(root_url + user_info_url + request_validators["tck"]);
    request.addHeader("Content-Type", "application/x-www-form-urlencoded");
    request.addHeader("Cookie", getCookies().toUtf8());

    QString data{"__VIEWSTATE="             + QUrl::toPercentEncoding(request_validators["__VIEWSTATE"])
                + "&__VIEWSTATEGENERATOR="  + request_validators["__VIEWSTATEGENERATOR"]
                + "&__EVENTVALIDATION="     + QUrl::toPercentEncoding(request_validators["__EVENTVALIDATION"])
                + "&TicketTextBox="         + cookies["ctck"]
                + "&Fm_Action=00&Frm_Type=&Frm_No=&XMLStdHlp=&TxtMiddle=%3Cr%2F%3E&ex="};

    return request.post(data.toUtf8());
}

void BriefInfoHandler::parseUserNumber(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &BriefInfoHandler::parseUserNumber);

    QString data, user_number;
    if (!verifyResponse(reply, data)) return;

    request_validators.insert(TextParser::Validators::extractFormValidators(data));
    user_number = TextParser::extractStudentNumber(data);
    if (user_number == QString()) {
        setErrorCode(Constants::Errors::ExtractError);
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

    QString data{"__VIEWSTATE="                  + QUrl::toPercentEncoding(request_validators["__VIEWSTATE"])
                + "&__VIEWSTATEGENERATOR="       + request_validators["__VIEWSTATEGENERATOR"]
                + "&__EVENTVALIDATION="          + QUrl::toPercentEncoding(request_validators["__EVENTVALIDATION"])
                + "&TicketTextBox="              + request_validators["tck"]
                + "&TxtMiddle=%3Cr+F41251%3D%22" + student_info["id"]
                + "%22%2F%3E&Fm_Action=08&Frm_Type=&Frm_No=&XMLStdHlp=&ex="};

    return request.post(data.toUtf8());
}

void BriefInfoHandler::parseUserInfo(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &BriefInfoHandler::parseUserInfo);
    QString data;
    if (!verifyResponse(reply, data)) return;

    if (!extractStudentInfo(data) || !extractStudentAvgs(data)) {
        setErrorCode(Constants::Errors::ExtractError);
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
    const QList<QString> keywords {"F51851",        // keyword for Name
                                   "F17551",        // Field
                                   "F41351",        // StudyType
                                   "F41701",        // TotalAvg
                                   "F41801"};      // Passed
    int position;
    QString value;
    // increased START_INDEX by 2 because we want to skip the Id cuz we don't have Id in this data.
    for (int title_index{INDEX_START + 2}, keyindex{0}; title_index < INDEX_END; ++title_index, ++keyindex) {
        position = response.indexOf(keywords[keyindex]);
        if (position == -1) {
            return false;
        }
        // 10 is the lentgh of actual keyword which for example is "F51851 = '"
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
    const QString year_keyword {"F4350"}, avg_keyword {"CumGPA"};
    year_position = response.indexOf(year_keyword);
    avg_position = response.indexOf(avg_keyword);
    if (year_position == -1 || avg_position == -1) {
        return false;
    }

    while (year_position != -1 && avg_position != -1) {
        // 7  and 8 are the lentgh of actual keywords which for example is 'F51851="'
        // so we need to skip this
        year_position += 7;
        avg_position += 8;
        for (int i{year_position}; response[i] != '"'; ++i) {
            year_value.append(response[i]);
        }

        for (int i{avg_position}; response[i] != ' '; ++i) {
            avg_value.append(response[i]);
        }
        passed_semesters.append(year_value.toInt());
        passed_semesters_avg.append(avg_value.toFloat());

        year_value.clear();
        avg_value.clear();
        year_position = response.indexOf(year_keyword, year_position);
        avg_position = response.indexOf(avg_keyword, avg_position);
    }
    return true;
}
