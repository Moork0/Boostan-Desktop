#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QSettings>
#include <QString>
#include <QVariant>
#include <QDir>

namespace Constants {

    inline const QString    application_name{"Boostan"},
                            organization_name{"AVID"},
                            domain_name{"SeedPuller.space"},
                            application_path {QDir::currentPath()};

    // default golestan url for inserting to settings if no configuration is available
    inline const QString    root_url {"https://golestan.umz.ac.ir"};
    inline QSettings        settings{QSettings::IniFormat, QSettings::UserScope, organization_name, application_name};

    /*
     * functions for work with settings
     */
    bool        checkSettings();
    QVariant    getConstant(const QString& key);
    void        setConstant(const QString& key, const QString& value);

    /*
     * Constants for errors
     */
    namespace Errors {

        enum {
            NoError                 = 0,
            CustomCode              = 500,
            UnknownError            = 1000,
            ServerConnenctionError  = CustomCode -1,
            WrongCaptcha            = CustomCode,
            CaptchaStoreError       = CustomCode + 1,
            SettingsError           = CustomCode + 2
        };

        inline const QHash<uint, QString> error_strings
        {
            {ServerConnenctionError, "خطا در اتصال به سرور"},
            {UnknownError, "اوه! یک خطای ناشناخته رخ داده!"},
            {WrongCaptcha, "کد امنیتی اشتباه وارد شده"},
            {CaptchaStoreError, "مشکلی در دریافت تصویر امنیتی بوجود اومده"},
            {SettingsError, "مشکلی در تنظیمات بوستان بوجود اومده"},
            {1, "نام کاربری یا رمز عبوری که وارد شده اشتباهه"}
        };

        inline const QHash<uint, QString> error_solutions
        {
            {ServerConnenctionError, "لطفا وضعیت اتصال به اینترنت و وبسایت گلستان رو بررسی کنید"},
            {UnknownError, "از اونجایی که منم نمیدونم چه خطاییه، بهتره لاگ هارو بررسی کنی و در صفحه گیتهاب این مشکل رو گزارش بدی"},
            {WrongCaptcha, "دوباره با دقت تلاش کن :)"},
            {CaptchaStoreError, "دسترسی های پوشه بوستان رو بررسی کنید و برنامه رو دوباره اجرا کنید"},
            {SettingsError, QString("دسترسی های فایل تنظیمات که در %1 قرار داره رو بررسی کنید").arg(settings.fileName())},
            {1, "دوباره با دقت تلاش کن :)"}
        };
    };
};

#endif // CONSTANTS_H
