#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H

#include "handler.h"
#include <iostream>

class LoginHandler : public Handler
{

private:
    const QByteArray image_path {"captcha.png"};
    const QString login_url{"/Forms/AuthenticateUser/AuthUser.aspx?fid=0;1&tck=&&&lastm=20190219160242"},
                  captcha_url{"/Forms/AuthenticateUser/captcha.aspx?0.03212876247262375"};
    QString student_number, student_name;

private slots:
    bool parseGetCaptcha(QNetworkReply& reply);
    bool parseLogin(QNetworkReply& reply);

public:
    LoginHandler();
    bool getCaptcha();
    bool tryLogin(const QString& username, const QString& password, const QString& captcha);

};

#endif // LOGINHANDLER_H
