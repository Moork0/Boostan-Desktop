#include "header/handler.h"
#include <iostream>

/*
 * initializing static variables
 */
const QString Handler::root_url {Constants::getConstant("root_url").toString()};
QHashString Handler::cookies, Handler::request_validators;

Handler::Handler(QObject *parent) : QObject(parent), is_finished{false}, success{false}, error_code{-1}
{
}


void Handler::setCookie(QString& key, QString& value)
{
    cookies[key] = value;
}

void Handler::setCookie(QString& keyvalue)
{
    QList<QString> splited = keyvalue.split('=');
    QString key {splited.takeFirst()};
    QString value {splited.join('=')};
    setCookie(key, value);
}

QString Handler::getCookies() const
{
    QString data;
    QHashString::const_iterator it = cookies.begin();
    for (; it != cookies.end(); ++it) {
        data += it.key() + "=" + it.value() + ";";
    }
    data.remove(data.size() - 1, 1);
    return data;
}

bool Handler::hasError(QNetworkReply::NetworkError ecode)
{
    setErrorCode(ecode);
    if (ecode == QNetworkReply::NoError) return false;
    return true;
}

bool Handler::getFinished() const
{
    return is_finished;
}

void Handler::setFinished(bool value)
{
    is_finished = value;
    if (is_finished == true) emit finished();
}

uint Handler::getErrorCode() const
{
    return error_code;
}

void Handler::setErrorCode(int ecode)
{
    if (error_code == ecode) return;
    error_code = ecode;
    emit errorCodeChanged();
}

QString Handler::getErrorString() const
{
    if (error_code >= QNetworkReply::ConnectionRefusedError && error_code <= QNetworkReply::UnknownServerError)
        return Constants::Errors::error_strings[Constants::Errors::ServerConnenctionError];

    return Constants::Errors::error_strings[error_code];
}

QString Handler::getErrorSolution() const
{
    if (error_code >= QNetworkReply::ConnectionRefusedError && error_code <= QNetworkReply::UnknownServerError)
        return Constants::Errors::error_solutions[Constants::Errors::ServerConnenctionError];

    return Constants::Errors::error_solutions[error_code];
}

void Handler::setSuccess(bool state)
{
    if (success == state) {
        return;
    }
    success = state;
    emit successChanged();
}

bool Handler::getSuccess() const
{
    return success;
}

void Handler::clearCookies()
{
    cookies.clear();
}

bool Handler::updateTokens(const QString& data)
{
    QHashString tokens {TextParser::extractTokens(data)};
    if (tokens.isEmpty()) return false;
    QHashString::iterator it {tokens.begin()};
    for (; it != tokens.end() && it.key() != "tck"; ++it) {
        cookies[it.key()] = it.value();
    }
    request_validators["tck"] = tokens["tck"];
    return true;
}







