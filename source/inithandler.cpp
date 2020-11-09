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

/*
    * parse init response.
    * 1- parse cookies and set ASP_SESSIONID to a valid value.
    * 2- extract validators.
*/
bool InitHandler::parseInit(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &InitHandler::parseInit);

    if (hasError(reply.error())) {
        reply.deleteLater();
        setSuccess(false);
        setFinished(true);
        return false;
    }

    QString data = reply.readAll();
    bool cookiefound{false};
    // extract cookies from response headers and add them to our 'cookies'.
    for (const auto& [key, value] : reply.rawHeaderPairs()) {
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
        //! TODO: here we should set error code to UnknownError
        reply.deleteLater();
        setSuccess(false);
        setFinished(true);
        return false;
    }

    request_validators = extractFormValidators(data);
    if (request_validators.empty()) {
        //! TODO: here we should set error code to UnknownError
        reply.deleteLater();
        setSuccess(false);
        setFinished(true);
        return false;
    }

    reply.deleteLater();
    setSuccess(true);
    setFinished(true);
    return true;
}
