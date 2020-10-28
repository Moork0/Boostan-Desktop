#include "header/captchahandler.h"

CaptchaHandler::CaptchaHandler()
{

}

bool CaptchaHandler::getCaptcha()
{
    connect(&request, &Network::complete, this, &CaptchaHandler::parseGetCaptcha);
    setFinished(false);
    request.setUrl(root_url + captcha_url);
    request.addHeader("Cookie", getCookies().toUtf8());
    return request.get();
}

bool CaptchaHandler::parseGetCaptcha(QNetworkReply& reply)
{
    disconnect(&request, &Network::complete, this, &CaptchaHandler::parseGetCaptcha);
    if (hasError(reply.error())) {
        reply.deleteLater();
        setSuccess(false);
        emit finished();
    }

    QFile file(image_path);
    if (!file.open(QIODevice::WriteOnly)) {
        reply.deleteLater();
        setSuccess(false);
        emit finished();
        return false;
    }

    file.write(reply.readAll());
    file.close();
    reply.deleteLater();
    setSuccess(true);
    emit finished();
    return true;
}
