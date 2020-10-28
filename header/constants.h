#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QDir>

namespace Constants {

    inline const QString    application_name{"Boostan"},
                            organization_name{"AVID"},
                            domain_name{"SeedPuller.space"},
                            application_path {QDir::currentPath() + "/"},
                            // default golestan url for inserting to settings if no configuration is available
                            root_url {"https://golestan.umz.ac.ir"},
                            settings_path {application_path + "settings.ini"};
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
            CustomCode              = 700,
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
            {CaptchaStoreError, "مشکلی در ذخیره تصویر امنیتی بوجود اومده"},
            {SettingsError, "مشکلی در تنظیمات بوستان بوجود اومده"},
            {1, "نام کاربری یا رمز عبوری که وارد شده اشتباهه"}
        };

        inline const QHash<uint, QString> error_solutions
        {
            {ServerConnenctionError, "لطفا وضعیت اتصال به اینترنت و وبسایت گلستان رو بررسی کنید"},
            {UnknownError, "از اونجایی که منم نمیدونم چه خطاییه، بهتره لاگ هارو بررسی کنی و در صفحه گیتهاب این مشکل رو گزارش بدی"},
            {WrongCaptcha, "دوباره با دقت تلاش کن :)"},
            {CaptchaStoreError, "دسترسی های پوشه بوستان رو بررسی کنید و برنامه رو دوباره اجرا کنید"},
            {SettingsError, "دسترسی های فایل تنظیمات که در پوشه برنامه قرار داره رو بررسی کنید"},
            {1, "دوباره با دقت تلاش کن :)"}
        };
    };
};

#endif // CONSTANTS_H
