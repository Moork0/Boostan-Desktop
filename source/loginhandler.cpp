#include "header/loginhandler.h"

LoginHandler::LoginHandler()
{

}

bool LoginHandler::getCaptcha()
{
    connect(&request, &Network::complete, this, &LoginHandler::parseGetCaptcha);
    setFinished(false);
    request.setUrl(root_url + captcha_url);
    request.addHeader("Cookie", getCookies().toUtf8());
    return request.get();
}

bool LoginHandler::parseGetCaptcha(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &LoginHandler::parseGetCaptcha);
    if (hasError(reply.error())) {
        reply.deleteLater();
        setSuccess(false);
        emit cFinished();
    }

    QFile file(image_path);
    if (!file.open(QIODevice::WriteOnly)) {
        reply.deleteLater();
        setSuccess(false);
        emit cFinished();
        return false;
    }

    file.write(reply.readAll());
    file.close();
    reply.deleteLater();
    setSuccess(true);
    emit cFinished();
    return true;
}

bool LoginHandler::tryLogin(const QString username, const QString password, const QString captcha)
{
    connect(&request, &Network::complete, this, &LoginHandler::parseLogin);
    setFinished(false);
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
    if (hasError(reply.error())) {
        reply.deleteLater();
        setSuccess(false);
        setFinished(true);
        return false;
    }
    const QString data {reply.readAll()};
    error_code = TextParser::hasError(data);
    if (error_code != Constants::Errors::NoError) {
        reply.deleteLater();
        setSuccess(false);
        setFinished(true);
        return false;
    }

    if (!updateTokens(data)) {
        setErrorCode(Constants::Errors::UnknownError);
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
