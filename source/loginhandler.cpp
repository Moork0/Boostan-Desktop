#include "header/loginhandler.h"

LoginHandler::LoginHandler()
{

}

bool LoginHandler::getCaptcha()
{
    connect(&request, &Network::complete, this, &LoginHandler::parseGetCaptcha);
    request.setUrl(root_url + captcha_url);
    request.addHeader("Cookie", getCookies().toUtf8());
    return request.get();
}

bool LoginHandler::parseGetCaptcha(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &LoginHandler::parseGetCaptcha);
    QFile file(image_path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(reply.readAll());
        file.close();
//        std::string c;
//        std::cout << "enter captcha: ";
//        std::cin >> c;
//        QString captcha(c.c_str());
//        tryLogin("username", "password", captcha);
        setSuccess(true);
        return true;
    }
    setSuccess(false);
    return false;
}

bool LoginHandler::tryLogin(const QString& username, const QString& password, const QString& captcha)
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
/*
 *
 *
 * FIX THIS FUNCTION
 *
 *
 */
bool LoginHandler::parseLogin(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &LoginHandler::parseLogin);
    const QString data {reply.readAll()};
    if (!updateTokens(data)) return false;
    student_name = TextParser::extractStudentName(data);
//    QFile file("res.html");
//    file.open(QIODevice::WriteOnly);
//    file.write(reply.readAll());
    return true;
}
