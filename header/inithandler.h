#ifndef INITHANDLER_H
#define INITHANDLER_H

#include "handler.h"
#include <QObject>

class InitHandler : public Handler
{
    Q_OBJECT
private:
    const QString loginurl{"/Forms/AuthenticateUser/AuthUser.aspx?fid=0;1&tck=&&&lastm=20190219160242"};

private slots:
    bool parseInit(QNetworkReply& reply);

public:
    InitHandler();
    Q_INVOKABLE bool start();
};

#endif // INITHANDLER_H
