#include "header/handlers/accounthandler.h"

AccountHandler::AccountHandler()
{

}

void AccountHandler::requestTokens()
{
    connect(&request, &Network::complete, this, &AccountHandler::parseTokens);
    // reset the status and continer and flag for every request
    setFinished(false);

    QString tck_token {getTckToken()};
    request.setUrl(_root_url + _account_url + tck_token);
    request.addHeader("Cookie", getCookies().toUtf8());
    request.get();
}

void AccountHandler::requestChangeCreds()
{
    connect(&request, &Network::complete, this, &AccountHandler::parseChangeCreds);

    request.setUrl(_root_url + _account_url + _request_validators[QStringLiteral("tck")]);
    request.addHeader("Content-Type", "application/x-www-form-urlencoded");
    request.addHeader("Cookie", getCookies().toUtf8());

    QString ticket_tbox {getTckToken()};
    QString param_txtmiddle {QString(QStringLiteral("<r MaxHlp=\"\" F51801=\"\" F80301=\"%1\" F80351=\"%2\" F51301=\"%3\" F51601=\"1\" F80751=\"\" F80431=\"\" F80352=\"%4\" F51302=\"%5\" F51303=\"%5\"/>")).arg(_request_validators[QStringLiteral("uid")], _username, _password, _new_username, _new_password)};
    QString data{
            QStringLiteral("__VIEWSTATE=")             % QUrl::toPercentEncoding(_request_validators["__VIEWSTATE"])
            % QStringLiteral("&__VIEWSTATEGENERATOR=")  % _request_validators["__VIEWSTATEGENERATOR"]
            % QStringLiteral("&__EVENTVALIDATION=")     % QUrl::toPercentEncoding(_request_validators["__EVENTVALIDATION"])
            % QStringLiteral("&TicketTextBox=")         % ticket_tbox
            % QStringLiteral("&TxtMiddle=")             % QUrl::toPercentEncoding(param_txtmiddle)
            % QStringLiteral("&Fm_Action=09&Frm_Type=&Frm_No=&XMLStdHlp=&ex=")
                };

    request.post(data.toUtf8());
}

void AccountHandler::parseTokens(QNetworkReply &reply)
{
    disconnect(&request, &Network::complete, this, &AccountHandler::parseTokens);
    QString data;
    if (!verifyResponse(reply, data)) {
        reply.deleteLater();
        setSuccess(false);
        setFinished(true);
        return;
    }

    reply.deleteLater();
    _request_validators.insert(extractFormValidators(data));
    requestChangeCreds();

}

void AccountHandler::parseChangeCreds(QNetworkReply &reply)
{
    disconnect(&request, &Network::complete, this, &AccountHandler::parseChangeCreds);
    bool parse_success {true};

    QString data;
    if (!verifyResponse(reply, data))
        parse_success = false;

    if (parse_success && !isChangeSuccess(data)) {
        setErrorCode(Errors::ExtractError);
        parse_success = false;
    }

    reply.deleteLater();
    if (!parse_success) {
        setSuccess(false);
        setFinished(true);
        return;
    }
    setSuccess(true);
    setFinished(true);
}

bool AccountHandler::isChangeSuccess(const QString &data)
{
    const QString key {MyStringLiteral("SuccArr = new Array('شن")};

    if (!data.contains(key))
        return false;

    return true;
}

void AccountHandler::changeCreds(const QString username, const QString password, const QString new_password, const QString new_username)
{
    _username = username;
    _password = password;
    _new_password = new_password;
    _new_username = new_username.isEmpty() ? username : new_username;
    requestTokens();
}
