#include "header/handler.h"

/*
 * initializing static variables
 */
const QString Handler::root_url {Settings::getValue("root_url").toString()};

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
    setErrorCode(ecode + Constants::Errors::qt_offset);
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
    QHashString tokens {TextParser::Validators::extractTokens(data)};
    if (tokens.isEmpty()) return false;
    QHashString::iterator it {tokens.begin()};
    for (; it != tokens.end() && it.key() != "tck"; ++it) {
        cookies[it.key()] = it.value();
    }
    request_validators["tck"] = tokens["tck"];
    return true;
}

bool Handler::verifyResponse(QNetworkReply& reply, QString& data)
{
    if (hasError(reply.error())) {
        reply.deleteLater();
        setSuccess(false);
        setFinished(true);
        return false;
    }
    if (data == QString()) data = reply.readAll();

    setErrorCode(TextParser::Errors::hasError(data));
    if (getErrorCode() != Constants::Errors::NoError) {
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
    return true;
}





