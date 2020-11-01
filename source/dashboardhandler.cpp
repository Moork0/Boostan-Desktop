#include "header/dashboardhandler.h"

DashboardHandler::DashboardHandler() : student_info {"", "", "", "", "", ""}
{

}

void DashboardHandler::start()
{
    getTokens();
}

bool DashboardHandler::getTokens()
{
    connect(&request, &Network::complete, this, &DashboardHandler::parseTokens);
    request.setUrl(root_url + user_info_url + request_validators["tck"]);
    request.addHeader("Cookie", getCookies().toUtf8());
    return request.get();
}

void DashboardHandler::parseTokens(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &DashboardHandler::parseTokens);
    QString data;
    if (!verifyResponse(reply, data)) return;

    request_validators.insert(TextParser::Validators::extractFormValidators(data));

    getUserNumber();
}

bool DashboardHandler::getUserNumber()
{
    connect(&request, &Network::complete, this, &DashboardHandler::parseUserNumber);
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

void DashboardHandler::parseUserNumber(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &DashboardHandler::parseUserNumber);

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
    student_info[user_info::Number] = user_number;
    getUserInfo();
}

bool DashboardHandler::getUserInfo()
{
    connect(&request, &Network::complete, this, &DashboardHandler::parseUserInfo);
    request.setUrl(root_url + user_info_url + request_validators["tck"]);
    request.addHeader("Content-Type", "application/x-www-form-urlencoded");
    request.addHeader("Cookie", getCookies().toUtf8());

    QString data{"__VIEWSTATE="                  + QUrl::toPercentEncoding(request_validators["__VIEWSTATE"])
                + "&__VIEWSTATEGENERATOR="       + request_validators["__VIEWSTATEGENERATOR"]
                + "&__EVENTVALIDATION="          + QUrl::toPercentEncoding(request_validators["__EVENTVALIDATION"])
                + "&TicketTextBox="              + request_validators["tck"]
                + "&TxtMiddle=%3Cr+F41251%3D%22" + student_info[Number]
                + "%22%2F%3E&Fm_Action=08&Frm_Type=&Frm_No=&XMLStdHlp=&ex="};

    return request.post(data.toUtf8());
}

void DashboardHandler::parseUserInfo(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &DashboardHandler::parseUserInfo);
    QString data;
    if (!verifyResponse(reply, data)) return;

    if (!extractStudentInfo(data) || !extractStudentAvgs(data)) {
        setErrorCode(Constants::Errors::ExtractError);
        setSuccess(false);
        setFinished(true);
        return;
    }
    qDebug() << student_info;
    qDebug() << student_avgs;
    setSuccess(true);
    setFinished(true);
    return;
}

bool DashboardHandler::extractStudentInfo(const QString& response)
{
    const QList<QString> keywords {"F51851",        // keyword for Name
                                   "F17551",        // Field
                                   "F41351",        // StudyType
                                   "F41701",        // TotalAvg
                                   "F41801"};      // Passed
    int position;
    QString value;
    for (QString keyword : keywords) {
        position = response.indexOf(keyword);
        if (position == -1) {
            return false;
        }
        // 10 is the lentgh of actual keyword which for example is "F51851 = '"
        // so we need to skip this
        position += 10;
        for (int i{position}; response[i] != "'"; ++i) {
            value.append(response[i]);
        }
        student_info.append(value);
        value.clear();
    }
    return true;
}

bool DashboardHandler::extractStudentAvgs(const QString &response)
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

        student_avgs.append(QPair<int, float>(year_value.toInt(), avg_value.toFloat()));
        year_value.clear();
        avg_value.clear();
        year_position = response.indexOf(year_keyword, year_position);
        avg_position = response.indexOf(avg_keyword, avg_position);
    }
    return true;
}
