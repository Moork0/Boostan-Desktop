#include "header/handlers/loginhandler.h"

LoginHandler::LoginHandler()
{
    // In this occasion, Unavailability of data is not acceptable.
    // So this error is Critical in this situation.
    error_handler.setCriticalStatus(18, Errors::Critical);
}

/*
     * prepare a request and send it to log in to Golestan system
*/
bool LoginHandler::tryLogin(const QString username, const QString password, const QString captcha)
{
    connect(&request, &Network::complete, this, &LoginHandler::parseLogin);
    // credentials would bind here
    QString logincreds = QString(QStringLiteral("<r F51851=\"\" F80351=\"%1\" F80401=\"%2\" F51701=\"%3\" F83181=\"\"/>")).arg(username, password, captcha);
    // data values should be in url-encoded format
    QString data{QStringLiteral("__VIEWSTATE=") % QUrl::toPercentEncoding(request_validators["__VIEWSTATE"])
                % QStringLiteral("&__VIEWSTATEGENERATOR=") % request_validators["__VIEWSTATEGENERATOR"]
                % QStringLiteral("&__EVENTVALIDATION=") % QUrl::toPercentEncoding(request_validators["__EVENTVALIDATION"])
                % QStringLiteral("&TxtMiddle=") % QUrl::toPercentEncoding(logincreds) % QStringLiteral("&Fm_Action=09&Frm_Type=&Frm_No=&TicketTextBox=")};
    request.setUrl(root_url + login_url);
    request.addHeader("Cookie", getCookies().toUtf8());
    request.addHeader("Content-Type", "application/x-www-form-urlencoded");
    return request.post(data.toUtf8());
}

bool LoginHandler::parseLogin(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &LoginHandler::parseLogin);
    QString data;
    bool parse_success {true};
    if (!verifyResponse(reply, data))
        parse_success = false;


    // extract student name
    if (parse_success && !extractName(data)) {
        setErrorCode(Errors::ExtractError);
        parse_success = false;
    }

    if (!parse_success) {
        reply.deleteLater();
        setSuccess(false);
        setFinished(true);
        return false;
    }
    reply.deleteLater();
    setSuccess(true);
    setFinished(true);
    return true;
}


bool LoginHandler::extractName(QString& response)
{
    const QString keyword {QStringLiteral("SetUsr('")};
    int position {response.indexOf(keyword)};
    if (position == -1) return false;
    // 8 is the length of "SetUsr('". we should skip this to capture the main value
    position += 8;
    QChar character;
    user_name.clear();
    // name in the response is like this: 'first name', 'lastname')
    // we skip the single-qoutes and at last remove the comma.
    while (response[position] != ")") {
        character = response[position];
        if (character != '\'') {
            user_name.append(character);
        }
        ++position;
    }
    user_name.replace(',', ' ');
    return true;
}

QString LoginHandler::getName() const
{
    return user_name;
}
