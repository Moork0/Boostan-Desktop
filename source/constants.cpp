#include "header/constants.h"

QVariant Constants::getConstant(const QString &key)
{
    return settings.value(key);
}

void Constants::setConstant(const QString &key, const QString &value)
{
    settings.setValue(key, value);
}

bool Constants::checkSettings()
{
    if (!settings.isWritable()) return false;
    if (!settings.contains("root_url"))
        settings.setValue("root_url", root_url);
    return true;
}
