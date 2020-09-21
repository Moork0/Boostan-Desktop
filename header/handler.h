#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QUrl>
#include <QFile>
#include <QHash>
#include <QDebug>

#include "network.h"
#include "textparser.h"
#include "constants.h"

class Handler : public QObject
{
    Q_OBJECT
protected:
    Q_PROPERTY(bool     finished        READ getFinished        NOTIFY finished)
    Q_PROPERTY(bool     success         READ getSuccess         NOTIFY successChanged)
    Q_PROPERTY(uint     errorCode       READ getErrorCode       NOTIFY errorCodeChanged)

    inline static QHashString      cookies, request_validators;
    static const  QString          root_url;

    Network                 request;
    bool                    is_finished, success;
    int                     error_code;

    void        setCookie(QString& key, QString& value);
    void        setCookie(QString& keyvalue);
    void        clearCookies();
    QString     getCookies() const;

    bool        hasError(QNetworkReply::NetworkError ecode);

    bool        getFinished() const;
    void        setFinished(bool value);

    uint        getErrorCode() const;
    void        setErrorCode(int ecode);

    void        setSuccess(bool state);
    bool        getSuccess() const;

public:
    explicit    Handler(QObject *parent = nullptr);
    bool        updateTokens(const QString& data);

signals:
    void finished();
    void successChanged();
    void errorCodeChanged();
};

#endif // HANDLER_H
