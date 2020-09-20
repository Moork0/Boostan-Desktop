#include "header/constants.h"

/*
 * Initializing Application Constant values and settings
 */
const QString Constants::application_name{"Boostan"},
              Constants::organization_name{"AVID"},
              Constants::domain_name{"SeedPuller.space"},
              Constants::application_path {QDir::currentPath()};

QSettings Constants::settings(QSettings::IniFormat, QSettings::UserScope, organization_name, application_name);

const QHash<uint, QString> Constants::Errors::error_strings
{
    {ServerConnenctionError, "خطا در اتصال به سرور"},
    {UnknownError, "اوه! یک خطای ناشناخته رخ داده!"},
    {CustomCode, "کد امنیتی اشتباه وارد شده"},
    {CustomCode + 1, "مشکلی در دریافت تصویر امنیتی بوجود اومده"},
    {1, "نام کاربری یا رمز عبوری که وارد شده اشتباهه"}
};

const QHash<uint, QString> Constants::Errors::error_solutions
{
    {ServerConnenctionError, "لطفا وضعیت اتصال به اینترنت و وبسایت گلستان رو بررسی کن"},
    {UnknownError, "از اونجایی که منم نمیدونم چه خطاییه، بهتره لاگ هارو بررسی کنی و در صفحه گیتهاب این مشکل رو گزارش بدی"},
    {CustomCode, "دوباره با دقت تلاش کن :)"},
    {CustomCode + 1, "دسترسی های پوشه بوستان رو بررسی کنید و برنامه رو دوباره اجرا کنید"},
    {1, "دوباره با دقت تلاش کن :)"}
};

/*
 *  Function implementations
 */
QVariant Constants::getConstant(const QString &key)
{
    return settings.value(key);
}

void Constants::setConstant(const QString &key, const QString &value)
{
    settings.setValue(key, value);
}
