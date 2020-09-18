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
    Q_PROPERTY(QString  errorString     READ getErrorString     NOTIFY errorCodeChanged)
    Q_PROPERTY(QString  errorSolution   READ getErrorSolution   NOTIFY errorCodeChanged)

    static const QString    root_url;
    static QHashString      cookies, request_validators;
    Network                 request;
    bool                    is_finished, success;
    int                     error_code;

    void        setCookie(QString& key, QString& value);
    void        setCookie(QString& keyvalue);
    void        clearCookies();
    QString     getCookies() const;

    bool        hasError(QNetworkReply::NetworkError ecode);

    bool getFinished() const;
    void setFinished(bool value);

    uint        getErrorCode() const;
    void        setErrorCode(int ecode);

    QString     getErrorString() const;
    void        setErrorString(const QString& str);

    QString     getErrorSolution() const;
    void        setErrorSolution(const QString& str);

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
