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
    QHashString::const_iterator it = cookies.cbegin();
    for (; it != cookies.cend(); ++it) {
        data += it.key() + "=" + it.value() + "; ";
    }
    data.chop(2);
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
    QHashString tokens {extractTokens(data)};
    if (tokens.isEmpty()) return false;
    QHashString::iterator it {tokens.begin()};
    cookies.remove("ctck");
    for (; it != tokens.end(); ++it) {
        if (it.key() == "tck") continue;
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
//    qDebug() << data;

    setErrorCode(extractDataError(data));
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

QHashString Handler::extractFormValidators(const QString& response)
{
    QHashString result;
    QRegularExpression re(viewstate_pattern);
    QRegularExpressionMatch match = re.match(response);
    QString capture;
    if (!match.hasMatch()) return QHashString {};
    capture = match.captured();
    capture.remove(capture.size() -1 , 1);
    capture.remove("__VIEWSTATE\" value=\"");
    result["__VIEWSTATE"] = capture;

    re.setPattern(viewstate_gen_pattern);
    match = re.match(response);
    if (!match.hasMatch()) return QHashString {};
    capture = match.captured();
    capture.remove("__VIEWSTATEGENERATOR\" value=\"");
    result["__VIEWSTATEGENERATOR"] = capture;

    re.setPattern(event_val_pattern);
    match = re.match(response);
    if (!match.hasMatch()) return QHashString {};
    capture = match.captured();
    capture.remove(capture.size() -1 , 1);
    capture.remove("__EVENTVALIDATION\" value=\"");
    result["__EVENTVALIDATION"] = capture;

//    qDebug() << result;
    return result;
}

QHashString Handler::extractTokens(const QString& response)
{
    QHashString tokens {{"u", ""}, {"su", ""}, {"ft", ""}, {"f", ""}, {"lt", ""}, {"ctck", ""}, {"seq", ""}, {"tck", ""}};
    QString capture;
    QRegularExpression re {tokens_pattern};
    QRegularExpressionMatch match {re.match(response)};

    if (!match.hasMatch()) return QHashString {};
    capture = match.captured().remove("SavAut(").remove("'");
    QStringList splited = capture.split(",");
    if (splited.size() < 7) return QHashString {};
    tokens["u"] = splited[0];
    tokens["su"] = splited[1];
    tokens["ft"] = splited[2];
    tokens["f"] = splited[3];
    tokens["lt"] = splited[4];
    tokens["ctck"] = splited[5];
    tokens["seq"] = splited[6];

    if (!response.contains("SetOpenerTck(") || response.contains("SetOpenerTck('')")) {
        tokens["tck"] = splited[5];
        tokens.remove("ctck");
    } else {
        re.setPattern(tck_pattern);
        match = re.match(response);
        if (!match.hasMatch()) return QHashString {};
        capture = match.captured().remove("SetOpenerTck('");
        tokens["tck"] = capture;
    }
    return tokens;
}

int Handler::extractDataErrorCode(const QString& response)
{
    int code_position {response.indexOf("کد")};
    QString code;
    if (code_position == -1) return Constants::Errors::NoCodeFound;

    int i = code_position + 2;
    while (response[i] != " ") {
        code.append(response[i]);
        ++i;
    }

    return code.toInt();
}

int Handler::extractDataError(const QString& response)
{
    if (response.contains("ErrorArr = new Array()")) return Constants::Errors::NoError;
    int code {extractDataErrorCode(response)};
    if (code != Constants::Errors::NoCodeFound) return code;
    QHash<int, QString>::const_iterator it {Constants::Errors::error_keywords.cbegin()};
    for (; it != Constants::Errors::error_keywords.cend(); ++it) {
        if (response.contains(it.value())) {
            return it.key();
        }
    }
    return Constants::Errors::UnknownError;
}
