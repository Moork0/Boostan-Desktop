#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include "constants.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    Settings();
    Q_INVOKABLE void    setValue(QString key, QString value);
    Q_INVOKABLE QString getValue(QString key);
};

#endif // SETTINGS_H
