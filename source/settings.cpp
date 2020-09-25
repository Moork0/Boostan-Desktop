#include "header/settings.h"

Settings::Settings()
{

}

void Settings::setValue(QString key, QString value)
{
    settings.setValue(key, value);
}

QVariant Settings::getValue(QString key)
{
    return settings.value(key);
}

bool Settings::checkSettings()
{
    if (!settings.isWritable()) return false;
    if (!settings.contains("root_url"))
        settings.setValue("root_url", Constants::root_url);
    return true;
}
