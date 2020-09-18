#include "header/inithandler.h"

InitHandler::InitHandler()
{

}

bool InitHandler::start()
{
    connect(&request, &Network::complete, this, &InitHandler::parseInit);
    request.setUrl(root_url + loginurl);
    return request.get();
}

bool InitHandler::parseInit(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &InitHandler::parseInit);

    if (hasError(reply.error())) {
        reply.deleteLater();
        setSuccess(false);
        return false;
    }

    QString data = reply.readAll();
    bool cookiefound{false};
    for (auto [key, value] : reply.rawHeaderPairs()) {
        if (key == "Set-Cookie") {
//            qDebug() << value;
            QString sid {value};
            sid = sid.split(';')[0];
            setCookie(sid);
            cookiefound = true;
            break;
        }
    }

    if (!cookiefound) {
        reply.deleteLater();
        setSuccess(false);
        return false;
    }

    request_validators = TextParser::extractLoginValidators(data);
    if (request_validators.empty()) {
        reply.deleteLater();
        setSuccess(false);
        return false;
    }
//    getCaptcha();
    reply.deleteLater();
    setSuccess(true);
    emit finished();
    return true;
}
