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
#include <QSGRendererInterface>

#include "header/errors.h"
#include "header/settings.h"
#include "header/inithandler.h"
#include "header/loginhandler.h"
#include "header/captchahandler.h"
#include "header/briefinfohandler.h"
#include "header/courseschedulehandler.h"

int main(int argc, char *argv[])
{

    //! Using RHI
     #if defined (Q_OS_WINDOWS)
         QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Direct3D12);
     #elif defined (Q_OS_MACOS)
         QQuickWindow::setSceneGraphBackend(QSGRendererInterface::MetalRhi);
     #elif defined (Q_OS_LINUX)
         QQuickWindow::setSceneGraphBackend(QSGRendererInterface::VulkanRhi);
     #endif

//    CourseScheduleHandler s;
//    QDir::setCurrent("/home/moorko/cpp/boostan/boostan/test/");
//    QFile file("courses.xml");
//    if (file.open(QIODevice::ReadOnly)) {
//        QString rr {file.readAll()};
//        s.extractWeeklySchedule(rr);
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
        universal_error_code = Constants::Errors::SettingsError;
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

    engine.rootContext()->setContextProperty("ApplicationPath", Constants::application_path);
    engine.rootContext()->setContextProperty("UniversalError", universal_error);
    engine.rootContext()->setContextProperty("UniversalErrorCode", universal_error_code);

    /** Types **/

    qmlRegisterType<InitHandler>("API.InitHandler", 1, 0, "InitHandler");
    qmlRegisterType<LoginHandler>("API.LoginHandler", 1, 0, "LoginHandler");
    qmlRegisterType<CaptchaHandler>("API.LoginHandler", 1, 0, "CaptchaHandler");
    qmlRegisterType<BriefInfoHandler>("API.BriefInfoHandler", 1, 0, "BriefInfoHandler");
    qmlRegisterType<CourseScheduleHandler>("API.CourseScheduleHandler", 1, 0, "CourseScheduleHandler");
    qmlRegisterType<Errors>("API.Errors", 1, 0, "Error");
    //! TODO: settings will be a non-creatable object and this should change to non creatable
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
