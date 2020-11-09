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
#include "constants.h"

class Settings : public QObject
{
    Q_OBJECT
private:
    inline static QSettings        settings{Constants::settings_path, QSettings::IniFormat};

public:
    Settings();
    // set value 'value' to key 'key'
    Q_INVOKABLE static void     setValue(QString key, QString value);
    // get value of key 'key'
    Q_INVOKABLE static QVariant getValue(QString key);
    // check if settings are available
    //! TODO: change this function to static
    bool                        checkSettings();
};

#endif // SETTINGS_H
