#ifndef CAPTCHAHANDLER_H
#define CAPTCHAHANDLER_H

/*
    * Class: CapthcaHandler
    * Files: captchahandler.h, captchahandler.cpp
    * This class handles requesting captcha picture
*/

#include "handler.h"

class CaptchaHandler : public Handler
{
    Q_OBJECT
private:
    const QString       captcha_url     {QStringLiteral("/Forms/AuthenticateUser/captcha.aspx?0.03212876247262375")};
    const QByteArray    image_path      {"captcha.png"};

private slots:
    // save captcha image to 'image_path'
    bool parseGetCaptcha(QNetworkReply& reply);

public:
    CaptchaHandler();
    // request a new captcah image
    Q_INVOKABLE bool getCaptcha();
};

#endif // CAPTCHAHANDLER_H
