#include "header/loginhandler.h"

LoginHandler::LoginHandler()
{

}

bool LoginHandler::tryLogin(const QString username, const QString password, const QString captcha)
{
    connect(&request, &Network::complete, this, &LoginHandler::parseLogin);
    QString logincreds = QString("<r F51851=\"\" F80351=\"%1\" F80401=\"%2\" F51701=\"%3\" F83181=\"\"/>").arg(username, password, captcha);
    QString data{"__VIEWSTATE=" + QUrl::toPercentEncoding(request_validators["__VIEWSTATE"])
                + "&__VIEWSTATEGENERATOR=" + request_validators["__VIEWSTATEGENERATOR"]
                + "&__EVENTVALIDATION=" + QUrl::toPercentEncoding(request_validators["__EVENTVALIDATION"])
                + "&TxtMiddle=" + QUrl::toPercentEncoding(logincreds) + "&Fm_Action=09&Frm_Type=&Frm_No=&TicketTextBox="};
    request.setUrl(root_url + login_url);
    request.addHeader("Cookie", getCookies().toUtf8());
    request.addHeader("Content-Type", "application/x-www-form-urlencoded");
    return request.post(data.toUtf8());
}

bool LoginHandler::parseLogin(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &LoginHandler::parseLogin);
    QString data;
    if (!verifyResponse(reply, data)) return false;
    if (!extractName(data)) {
        setErrorCode(Constants::Errors::ExtractError);
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
    const QString keyword {"SetUsr('"};
    int position {response.indexOf(keyword)};
    if (position == -1) return false;
    position += 8;
    QChar character;
    user_name.clear();
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
