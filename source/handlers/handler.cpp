#include "header/handlers/handler.h"


Handler::Handler(QObject *parent) : QObject(parent), is_finished{false}, success{false}, error_code{-1}
{
    root_url = Settings::getValue("root_url").toString();
}

void Handler::setCookie(QString& key, QString& value)
{
    cookies[key] = value;
}

/*
    * split 'keyvalue' string into key,value and pass them to setCookie()
*/
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
    // since the ecode is one of the Qt default codes, we should add it to qt_offset
    // to prevent conflict with Golestan error codes.
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
    // we only wanna use finished() when an request is finished.
    if (is_finished == true) emit finished();
    emit workingChanged();
}

bool Handler::getWorking() const
{
    return !is_finished;
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
    if (success == state) return;
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
    // we should remove 'ctck' at every update
    // because we should use ctck only when Golestan says.
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
        // we don't know what will gonna prevent updateTokens() to not updating tokens.
        // so the error is unknown and no more progress can be done.
        setErrorCode(Constants::Errors::UnknownError);
        reply.deleteLater();
        setSuccess(false);
        setFinished(true);
        return false;
    }

    return true;
}

/**
    * TODO: since regexes are generally slower than normal string searchs,
    * i should compare the performance of this function when using REGEXes and
    * when using string searchs. then choose the fastest one.
**/
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
    //! TODO: idk if i should move this variable to class data members or not
    //! cuz this function would called alot and constructing this variable
    //! every time the function being called would be non-optimal.
    // tokens that Golestan will return at every request and we need these to be able to make
    // another requests.
    QHashString tokens {{"u", ""}, {"su", ""}, {"ft", ""}, {"f", ""}, {"lt", ""}, {"ctck", ""}, {"seq", ""}, {"tck", ""}};
    QString capture;
    QRegularExpression re {tokens_pattern};
    QRegularExpressionMatch match {re.match(response)};

    if (!match.hasMatch()) return QHashString {};
    capture = match.captured().remove("SavAut(").remove("'");
    QStringList splited = capture.split(",");
    // tokens.size() - 1(we dont wanna tck now) = 7
    if (splited.size() < 7) return QHashString {};
    /// this could be done with a loop. but it's unnecessary i think.
    tokens["u"] = splited[0];
    tokens["su"] = splited[1];
    tokens["ft"] = splited[2];
    tokens["f"] = splited[3];
    tokens["lt"] = splited[4];
    tokens["ctck"] = splited[5];
    tokens["seq"] = splited[6];

    /*
        * Normally, 'tck' and 'ctck' are equal to each other and in this case, Golestan only needs 'tck'.
        * But sometimes Golestan explicitly returns tck in other way. in that case we use both 'tck' and 'ctck'
    */
    // check if 'tck' is explicitly defined
    if (!response.contains("SetOpenerTck(") || response.contains("SetOpenerTck('')")) {
        // no 'tck' defined explicitly. use 'ctck' instead and remove 'ctck' from tokens.
        tokens["tck"] = splited[5]; // splited[5] == ctck
        tokens.remove("ctck");
    } else {
        // 'tck' is defined explicitly. we extract that.
        //! TODO: this could be done with string search and would be faster than regex.
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
    // all error codes will come after the word 'code'(in persian)
    int code_position {response.indexOf("کد")};
    QString code;
    if (code_position == -1) return Constants::Errors::NoCodeFound;

    // 2 is the length of 'code' in persian. we should skip this to capture actual value.
    int i = code_position + 2;
    while (response[i] != " ") {
        code.append(response[i]);
        ++i;
    }

    return code.toInt();
}
/*
    * This function at first try to extract code from the response.
    * if no code found, then try to find a key word that matches the custom error key words.
*/
int Handler::extractDataError(const QString& response)
{
    if (response.contains("ErrorArr = new Array()")) return Constants::Errors::NoError;
    int code {extractDataErrorCode(response)};
    if (code != Constants::Errors::NoCodeFound) return code;
    QHash<int, QString>::const_iterator it {Constants::Errors::error_keywords.cbegin()};
    for (; it != Constants::Errors::error_keywords.cend(); ++it) {
        if (response.contains(it.value())) {
            // key is a custom error code.
            return it.key();
        }
    }
    // code has error but no corresponding custom error found.
    return Constants::Errors::UnknownError;
}
