#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QDir>

using QHashString = QHash <QString, QString>;

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

        inline constexpr int qt_offset{100};
        inline constexpr int NoCodeFound{-1};

        enum error_codes{
            NoError                 = 0,
            CustomCode              = 800,
            UnknownError            = 1000,
            ServerConnenctionError  = CustomCode -1,
            WrongCaptcha            = CustomCode,
            CaptchaStoreError       = CustomCode + 1,
            SettingsError           = CustomCode + 2,
            ExtractError            = CustomCode + 3
        };

        enum error_type{
            Critical = 0,
            SemiCritical,
            Normal
        };

        inline const QHash<int, int> critical_status
        {
            {ServerConnenctionError, SemiCritical},
            {UnknownError, Critical},
            {WrongCaptcha, Normal},
            {CaptchaStoreError, Critical},
            {SettingsError, Critical},
            {ExtractError, Normal},
            // this is Golestan error codes that we might see.
            {1, Normal},
            {24, SemiCritical},
            {27, SemiCritical}
        };

        inline const QHash<int, QString> error_keywords {{WrongCaptcha, "كد امنيتي"}};

        inline const QHash<uint, QString> error_strings
        {
            {ServerConnenctionError, "خطا در اتصال به سرور"},
            {UnknownError, "اوه! یک خطای ناشناخته رخ داده!"},
            {WrongCaptcha, "کد امنیتی اشتباه وارد شده!"},
            {CaptchaStoreError, "مشکلی در ذخیره تصویر امنیتی بوجود اومده!"},
            {SettingsError, "مشکلی در تنظیمات بوستان بوجود اومده!"},
            {ExtractError, "مشکلی در استخراج اطلاعات از گلستان بوجود اومده!"},
            // this is Golestan error codes that we might see.
            {1, "نام کاربری یا رمز عبوری که وارد شده اشتباهه!"},
            {24, "گلستان میگه دوباره باید وارد بشی!"},
            {27, "تعداد تلاش ناموفق برای ورود بیش از حد مجاز شده!"}
        };

        inline const QHash<uint, QString> error_solutions
        {
            {ServerConnenctionError, "لطفا وضعیت اتصال به اینترنت و وبسایت گلستان رو بررسی کنید"},
            {UnknownError, "از اونجایی که منم نمیدونم چه خطاییه، بهتره لاگ هارو بررسی کنی و در صفحه گیتهاب این مشکل رو گزارش بدی"},
            {WrongCaptcha, "دوباره با دقت تلاش کن :)"},
            {CaptchaStoreError, "دسترسی های پوشه بوستان رو بررسی کنید و برنامه رو دوباره اجرا کنید"},
            {SettingsError, "دسترسی های فایل تنظیمات که در پوشه برنامه قرار داره رو بررسی کنید"},
            {ExtractError, "یکبار دیگه تلاش کن تا بوستان این مشکل رو برات حل کنه"},
            // this is Golestan error codes that we might see.
            {1, "دوباره با دقت تلاش کن :)"},
            {24, "دوباره برای ورود تلاش کن تا این مشکل رفع بشه. اگر نشد لطفا در گیتهاب خبر بده"},
            {27, "یکبار دیگه تلاش کن تا بوستان این مشکل رو برات حل کنه"}
        };
    };
};

#endif // CONSTANTS_H
