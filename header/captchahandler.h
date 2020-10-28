#ifndef CAPTCHAHANDLER_H
#define CAPTCHAHANDLER_H

#include "header/handler.h"

class CaptchaHandler : public Handler
{
    Q_OBJECT
private:
    const QString       captcha_url     {"/Forms/AuthenticateUser/captcha.aspx?0.03212876247262375"};
    const QByteArray    image_path      {"captcha.png"};

private slots:
    bool parseGetCaptcha(QNetworkReply& reply);

public:
    CaptchaHandler();
    Q_INVOKABLE bool getCaptcha();
};

#endif // CAPTCHAHANDLER_H
