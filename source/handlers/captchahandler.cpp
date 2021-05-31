#include "header/handlers/captchahandler.h"

CaptchaHandler::CaptchaHandler()
{

}

bool CaptchaHandler::getCaptcha()
{
    connect(&_request, &Network::complete, this, &CaptchaHandler::parseGetCaptcha);
    setFinished(false);
    _request.setUrl(_root_url + _captcha_url);
    _request.addHeader("Cookie", getCookies().toUtf8());
    return _request.get();
}

bool CaptchaHandler::parseGetCaptcha(QNetworkReply& reply)
{
    disconnect(&_request, &Network::complete, this, &CaptchaHandler::parseGetCaptcha);
    if (hasError(reply.error())) {
        reply.deleteLater();
        setSuccess(false);
        setFinished(true);
        return false;
    }

    QFile file(_image_path);
    if (!file.open(QIODevice::WriteOnly)) {
        setErrorCode(Errors::CaptchaStoreError);
        reply.deleteLater();
        setSuccess(false);
        setFinished(true);
        return false;
    }

    file.write(reply.readAll());
    file.close();

    reply.deleteLater();
    setSuccess(true);
    setFinished(true);
    return true;
}
