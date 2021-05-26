#ifndef SETTINGS_H
#define SETTINGS_H

/*
    * Class: Settings
    * Files: settings.h and settings.cpp
    * This class is a wrapper on QSettings and suppose to check if settings are available
    * and set settings value and retrieve them
*/

#include <QObject>
#include <QSettings>
#include <QStringBuilder>
#include "../helpers/constants.h"

class Settings : public QObject
{
    Q_OBJECT
private:
    inline static QSettings         settings{Constants::settings_path, QSettings::IniFormat};
    inline static QString           prefix_uid;
    inline static QString           prefix_url;

public:
    Settings();
    // check if settings are available
    static bool                 checkSettings();

public slots:
    // set value 'value' to key 'key'
    static void     setValue(const QString key, const QString value, const bool raw_key = false);
    // get value of key 'key'. raw_key defines if we should include prefixes in key or not.
    static QVariant getValue(const QString key, const bool raw_key = false);
    // set uid to prefix_uid
    static void     setPrefixUid(const QString uid);
    // set url to prefix_url
    static void     setPrefixUrl(const QString url);
};

#endif // SETTINGS_H
