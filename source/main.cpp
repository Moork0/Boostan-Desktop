#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSGRendererInterface>
#include <QQuickWindow>
#include <QDebug>
#include "header/textparser.h"
#include "header/errors.h"
#include "header/settings.h"
#include "header/inithandler.h"
#include "header/loginhandler.h"
#include "header/captchahandler.h"
#include "header/briefinfohandler.h"

//bool f(QString& response)
//{


//}

int main(int argc, char *argv[])
{

    //!Using RHI
     #if defined (Q_OS_WINDOWS)
         QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Direct3D12);
     #elif defined (Q_OS_MACOS)
         QQuickWindow::setSceneGraphBackend(QSGRendererInterface::MetalRhi);
     #elif defined (Q_OS_LINUX)
         QQuickWindow::setSceneGraphBackend(QSGRendererInterface::VulkanRhi);
     #endif

//    QDir::setCurrent("/home/moorko/cpp/boostan/boostan/test/");
//    QFile file("res2.html");
//    if (file.open(QIODevice::ReadOnly)) {
//        QString rr {file.readAll()};
//        f(rr);
//    } else {
//        qDebug() << file.errorString();
//    }

    bool universal_error{false};
    int universal_error_code {0};
    Settings settings;
    if (!settings.checkSettings()) {
        universal_error = true;
        universal_error_code = Constants::Errors::SettingsError;
    }


    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    app.setOrganizationName(Constants::application_name);
    app.setOrganizationDomain(Constants::domain_name);
    app.setApplicationName(Constants::organization_name);

    QQmlApplicationEngine engine;
    /*
     * QML exposures
    */
    engine.rootContext()->setContextProperty("ApplicationPath", Constants::application_path);
    engine.rootContext()->setContextProperty("UniversalError", universal_error);
    engine.rootContext()->setContextProperty("UniversalErrorCode", universal_error_code);
    qmlRegisterType<InitHandler>("API.InitHandler", 1, 0, "InitHandler");
    qmlRegisterType<LoginHandler>("API.LoginHandler", 1, 0, "LoginHandler");
    qmlRegisterType<CaptchaHandler>("API.LoginHandler", 1, 0, "CaptchaHandler");
    qmlRegisterType<BriefInfoHandler>("API.BriefInfoHandler", 1, 0, "BriefInfoHandler");
    qmlRegisterType<Errors>("API.Errors", 1, 0, "Error");
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
