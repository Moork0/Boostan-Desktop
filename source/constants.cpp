#include "header/constants.h"

/*
 * Initializing Application Constant values and settings
 */
const QString Constants::application_name{"Boostan"},
              Constants::organization_name{"AVID"},
              Constants::domain_name{"SeedPuller.space"},
              Constants::application_path {QDir::currentPath()};


const QHash<uint, QString> Constants::Errors::error_strings
{
    {Constants::Errors::ServerConnenctionError, "خطا در اتصال به سرور"}
};

const QHash<uint, QString> Constants::Errors::error_solutions
{
    {Constants::Errors::ServerConnenctionError, "لطفا وضعیت اتصال به اینترنت و وبسایت گلستان رو بررسی کنید"}
};

/*
 *  Function implementations
 */
QSettings Constants::settings(QSettings::IniFormat, QSettings::UserScope, organization_name, application_name);

QVariant Constants::getConstant(const QString &key)
{
    return settings.value(key);
}

void Constants::setConstant(const QString &key, const QString &value)
{
    settings.setValue(key, value);
}
