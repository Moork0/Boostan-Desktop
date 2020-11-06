#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QUrl>
#include <QFile>
#include <QHash>
#include <QDebug>
#include <QRegularExpression>

#include "network.h"
#include "constants.h"
#include "settings.h"

class Handler : public QObject
{
    Q_OBJECT
private:
    const QString viewstate_pattern         {"__VIEWSTATE\" value=\"\\S+"};                     // regex patern for view state
    const QString viewstate_gen_pattern     {"__VIEWSTATEGENERATOR\" value=\"[A-Za-z0-9]+"};   // regex pattern for viewstate generator
    const QString event_val_pattern         {"__EVENTVALIDATION\" value=\"\\S+"};             // regex pattern for event validation
    const QString tokens_pattern            {"SavAut\\([,a-z0-9'A-Z-]+"};                    // regex pattern for lt,ctck,u,ft,seq and ...
    const QString tck_pattern               {"SetOpenerTck\\('[A-Z0-9-]{16}"};              // regex pattern for tck.
    const QString error_withcode_pattern    {"ErrorArr = new Array\\('[\\w :]+"};          // regex pattern for finding error which has a code

    QHashString extractTokens(const QString& response);
    int extractDataErrorCode(const QString& response);
    int extractDataError(const QString& response);

protected:
    Q_PROPERTY(bool     finished        READ getFinished        NOTIFY finished)
    Q_PROPERTY(bool     success         READ getSuccess         NOTIFY successChanged)
    Q_PROPERTY(uint     errorCode       READ getErrorCode       NOTIFY errorCodeChanged)
    Q_PROPERTY(bool     working         READ getWorking         NOTIFY workingChanged)

    inline static QHashString      cookies, request_validators;
    static const  QString          root_url;

    Network                 request;
    bool                    is_finished, success;
    int                     error_code;

    void        setCookie(QString& key, QString& value);
    void        setCookie(QString& keyvalue);
    void        clearCookies();
    QString     getCookies() const;

    bool        getFinished() const;
    void        setFinished(bool value);
    bool        getWorking() const;

    uint        getErrorCode() const;
    void        setErrorCode(int ecode);

    void        setSuccess(bool state);
    bool        getSuccess() const;

    bool        hasError(QNetworkReply::NetworkError ecode);
    bool        updateTokens(const QString& data);
    bool        verifyResponse(QNetworkReply& reply, QString& data);

    QHashString extractFormValidators(const QString& response);

public:
    explicit    Handler(QObject *parent = nullptr);

signals:
    void finished();
    void successChanged();
    void errorCodeChanged();
    void workingChanged();
};

#endif // HANDLER_H
