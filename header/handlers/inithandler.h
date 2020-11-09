#ifndef INITHANDLER_H
#define INITHANDLER_H

/*
    * Class: InitHandler
    * Files: inithandler.h, inithandler.cpp
    * This class is supposed to initialize an connection between client
    * and Golestan system.
*/

#include "handler.h"
#include <QObject>

class InitHandler : public Handler
{
    Q_OBJECT
private:
    const QString loginurl{"/Forms/AuthenticateUser/AuthUser.aspx?fid=0;1&tck=&&&lastm=20190219160242"};

private slots:
    // extract session id and parse informations needed to make subsequent requests.
    bool parseInit(QNetworkReply& reply);

public:
    // send application's first request
    InitHandler();
    Q_INVOKABLE bool start();
};

#endif // INITHANDLER_H
