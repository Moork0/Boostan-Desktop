#ifndef SETTINGS_H
#define SETTINGS_H

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
    Q_INVOKABLE static void     setValue(QString key, QString value);
    Q_INVOKABLE static QVariant getValue(QString key);
    bool                        checkSettings();
};

#endif // SETTINGS_H
