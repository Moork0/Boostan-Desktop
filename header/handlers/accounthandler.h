#ifndef ACCOUNTHANDLER_H
#define ACCOUNTHANDLER_H

#include "handler.h"
#include <QObject>

class AccountHandler : public Handler
{
    Q_OBJECT

private:
    const QString _account_url {QStringLiteral("/Forms/F0217_PROCESS_SCROLDPASS/F0217_01_PROCESS_SCROLDPASS_Dat.aspx?r=0.8555344031558193&fid=0;11160&b=0&l=0&&lastm=20190220153948&tck=")};
    QString _username, _password, _new_password, _new_username;

    void    requestTokens       ();
    void    requestChangeCreds  ();

    bool    isChangeSuccess     (const QString& data);

private slots:
    // parse the response to requestTokens()
    void    parseTokens         (QNetworkReply& reply);
    // parse the response to requestScores()
    void    parseChangeCreds    (QNetworkReply& reply);

public:
    AccountHandler();

public slots:
    void changeCreds            (const QString username, const QString password,
                                 const QString new_password, const QString new_username = QString());

};

#endif // ACCOUNTHANDLER_H
