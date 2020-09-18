#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
//#include <QFile>
//#include <QDir>
//#include <QUrl>
#include <QDebug>
//#include "handler.h"
#include "header/textparser.h"
#include "header/inithandler.h"
#include "header/loginhandler.h"

int main(int argc, char *argv[])
{

//    TextParser parser;

//    login.getCaptcha();
//    QDir::setCurrent("/home/moorko/cpp/boostan-test/boostan-test");
//    QFile file("courses.xml");
//    if (file.open(QIODevice::ReadOnly)) {
//        QString rr {file.readAll()};
//        TextParser::extractOfferedCourses(rr);
//    } else {
//        qDebug() << file.errorString();
//    }
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
    qmlRegisterType<InitHandler>("API.InitHandler", 1, 0, "InitHandler");
    qmlRegisterType<LoginHandler>("API.LoginHandler", 1, 0, "LoginHandler");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
    [url](QObject *obj, const QUrl &objUrl){
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
