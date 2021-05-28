/*
    * This file handles everything.
    * QML exposures occur here.
    * Application initialization occur here.
*/

#include <QtWidgets/QApplication>
//#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>

#include "header/helpers/errors.h"
#include "header/base/settings.h"
//! handlers
#include "header/handlers/inithandler.h"
#include "header/handlers/loginhandler.h"
#include "header/handlers/captchahandler.h"
#include "header/handlers/briefinfohandler.h"
#include "header/handlers/courseschedulehandler.h"
#include "header/handlers/offeredcoursehandler.h"
#include "header/handlers/scoreshandler.h"
#include "header/handlers/accounthandler.h"
//! models
#include "header/models/offeredcoursemodel.h"
//! Controls
#include "header/controls/scheduletable.h"

/// TODO: use QString::at() instead of [] for readonly purpose

int main(int argc, char *argv[])
{

//    QDir::setCurrent("/home/moorko/cpp/boostan/boostan/test/");
//    QFile file("res.html");
//    if (file.open(QIODevice::ReadOnly)) {
//        QString rr {file.readAll()};
//        OfferedCourseHandler obj;
//        obj.extractOfferedCourses(rr);
//    } else {
//        qDebug() << file.errorString();
//    }

    bool universal_error{false};
    int universal_error_code {0};
    //! TODO: Settings will be just a static class and this shall remove
    Settings settings;
    // check if settings are available
    if (!settings.checkSettings()) {
        universal_error = true;
        universal_error_code = Errors::SettingsError;
    }

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
//    QGuiApplication app(argc, argv);
    app.setOrganizationName(Constants::application_name);
    app.setOrganizationDomain(Constants::domain_name);
    app.setApplicationName(Constants::organization_name);

    QQmlApplicationEngine engine;

    // QML exposures

    /** Constants **/

    engine.rootContext()->setContextProperty(QStringLiteral("ApplicationPath"), Constants::application_path);
    engine.rootContext()->setContextProperty(QStringLiteral("UniversalError"), universal_error);
    engine.rootContext()->setContextProperty(QStringLiteral("UniversalErrorCode"), universal_error_code);

    /** Types **/

    /*  Controls back-end  */
    qmlRegisterType <ScheduleTable>             ("API.Controls.ScheduleTable", 1, 0, "ScheduleTableBackEnd");

    /* Handler types */
    qmlRegisterType <InitHandler>               ("API.InitHandler",             1, 0, "InitHandler");
    qmlRegisterType <LoginHandler>              ("API.LoginHandler",            1, 0, "LoginHandler");
    qmlRegisterType <CaptchaHandler>            ("API.LoginHandler",            1, 0, "CaptchaHandler");
    qmlRegisterType <BriefInfoHandler>          ("API.BriefInfoHandler",        1, 0, "BriefInfoHandler");
    qmlRegisterType <CourseScheduleHandler>     ("API.CourseScheduleHandler",   1, 0, "CourseScheduleHandler");
    qmlRegisterType <OfferedCourseHandler>      ("API.OfferedCourseHandler",    1, 0, "OfferedCourseHandler");
    qmlRegisterType <ScoresHandler>             ("API.ScoresHandler",           1, 0, "ScoresHandler");
    qmlRegisterType <AccountHandler>            ("API.AccountHandler",          1, 0, "AccountHandler");


    /*   Models   */
    qmlRegisterType <OfferedCourseModel>        ("API.OfferedCourseHandler",    1, 0, "OfferedCourseModel");

    /*   Helper types   */
    qmlRegisterType <Errors>                    ("API.Errors",                  1, 0, "Error");
    /// TODO: settings will be a non-creatable object and this should change to non creatable
    qmlRegisterSingletonInstance("API.Settings", 1, 0, "Settings", &settings);


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
    [url](QObject *obj, const QUrl &objUrl){
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
