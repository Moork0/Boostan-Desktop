#include "header/base/settings.h"

Settings::Settings()
{

}

void Settings::setValue(const QString key, const QString value)
{
    settings.setValue(prefix_url % QStringLiteral("/") % prefix_uid % QStringLiteral("/") % key, value);
}

QVariant Settings::getValue(const QString key, const bool raw_key)
{
    if (raw_key)
        return settings.value(key);
    return settings.value(prefix_url % QStringLiteral("/") % prefix_uid % QStringLiteral("/") % key);
}

void Settings::setPrefixUid(const QString uid)
{
    prefix_uid = uid;
}

void Settings::setPrefixUrl(const QString url)
{
    prefix_url = url;
}

// check if settings is writable and has some required default value
bool Settings::checkSettings()
{
    if (!settings.isWritable()) return false;
    if (!settings.contains("root_url"))
        settings.setValue("root_url", Constants::root_url);

    setPrefixUrl(settings.value(QStringLiteral("root_url")).toString());
    return true;
}
