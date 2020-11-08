#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H

/*
 * Class: LoginHandler
 * Files: loginhandler.h, loginhandler.cpp
 * This class handle login things and extract student name after successful login.
 */

#include "handler.h"

class LoginHandler : public Handler
{
    Q_OBJECT
private:
    const QString   login_url{"/Forms/AuthenticateUser/AuthUser.aspx?fid=0;1&tck=&&&lastm=20190219160242"};
    QString         user_name;

    // extract student name and store it into 'user_name' by parsing 'response'
    bool            extractName (QString& response);

private slots:
    // return true if login was successful by parsing 'reply'
    bool            parseLogin  (QNetworkReply& reply);

public:
    LoginHandler();
    // send a request for login
    Q_INVOKABLE bool    tryLogin(const QString username, const QString password, const QString captcha);
    // return 'user_name'
    Q_INVOKABLE QString getName() const;
};

#endif // LOGINHANDLER_H
