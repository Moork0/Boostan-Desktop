#ifndef ACCOUNTHANDLER_H
#define ACCOUNTHANDLER_H

/*
    * Class: AccountHandler
    * Files: accounthandler.h, accounthandler.cpp
    * The task of this class is to change the Golestan account credentials
*/

#include "handler.h"

class AccountHandler : public Handler
{
    Q_OBJECT

private:
    const QString _account_url {QStringLiteral("/Forms/F0217_PROCESS_SCROLDPASS/F0217_01_PROCESS_SCROLDPASS_Dat.aspx?r=0.8555344031558193&fid=0%3b11160&b=0&l=0&&lastm=20190220153948&tck=")};
    QString _username, _password, _new_password, _new_username;

    // request golestan to get the tokens
    void    requestTokens       ();
    // request the golestan to change the credentials
    void    requestChangeCreds  ();

    // check if credential changing proccess is successfull
    bool    isChangeSuccess     (const QString& data);

private slots:
    // parse the response to requestTokens()
    void    parseTokens         (QNetworkReply& reply);
    // parse the response to requestChangeCreds()
    void    parseChangeCreds    (QNetworkReply& reply);

public:
    AccountHandler();

public slots:
    // change the credentials of account with username {{username}} and password {{password}}
    // and change them to {{new_username}} and {{new_password}}
    void changeCreds            (const QString username, const QString password,
                                 const QString new_password, const QString new_username = QString());

};

#endif // ACCOUNTHANDLER_H
