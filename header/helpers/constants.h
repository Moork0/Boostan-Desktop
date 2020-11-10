#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
    * file name: Constatns.h
    * This file suppose to store all of the constant values which other files might use.
*/

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
        * Constants for errors.
        * I doubt if this values should be here or not. i'm thinking about moving them to errors.h
        * because these values are explicitly used for errors and we have such a class and by moving
        * them to errors.h, related data would be near together.
    */

    namespace Errors {

        /*
            * 'qt_offset' is offset for QNetworkReply::NetworkError codes
            * the reason i defined this is Golestan error codes are in conflict with Qt network error codes.
        */
        inline constexpr int qt_offset{100};
        inline constexpr int NoCodeFound{-1};

        // our custom error codes.
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

        // error types that identify if a error code is critical or not
        enum error_type{
            Critical = 0,
            SemiCritical,
            Normal
        };

        // type of each error code
        inline const QHash<int, int> critical_status
        {
            {ServerConnenctionError, SemiCritical},
            {UnknownError, Critical},
            {WrongCaptcha, Normal},
            {CaptchaStoreError, Critical},
            {SettingsError, Critical},
            {ExtractError, SemiCritical},
            // this is built-in Golestan error codes that we might see.
            {1, Normal},
            {24, SemiCritical},
            {27, SemiCritical}
        };

        /*
            * Key words for identifying an error code for strings that returned by Golestan
            * and has no built-in error codes in it.
        */
        inline const QHash<int, QString> error_keywords
        {
            {WrongCaptcha, "كد امنيتي"}     // error strings that has this keyword are about WrongCaptcha
        };

        // Error title for each error code that identifyed so far
        inline const QHash<uint, QString> error_strings
        {
            {ServerConnenctionError, "خطا در اتصال به سرور"},
            {UnknownError, "اوه! یک خطای ناشناخته رخ داده!"},
            {WrongCaptcha, "کد امنیتی اشتباه وارد شده!"},
            {CaptchaStoreError, "مشکلی در ذخیره تصویر امنیتی بوجود اومده!"},
            {SettingsError, "مشکلی در تنظیمات بوستان بوجود اومده!"},
            {ExtractError, "مشکلی در استخراج اطلاعات از گلستان بوجود اومده!"},
            // this is built-in Golestan error codes that we might see.
            {1, "نام کاربری یا رمز عبوری که وارد شده اشتباهه!"},
            {24, "گلستان میگه دوباره باید وارد بشی!"},
            {27, "تعداد تلاش ناموفق برای ورود بیش از حد مجاز شده!"}
        };

        // Error description for each error code that identifyed so far
        inline const QHash<uint, QString> error_solutions
        {
            {ServerConnenctionError, "لطفا وضعیت اتصال به اینترنت و وبسایت گلستان رو بررسی کنید"},
            {UnknownError, "از اونجایی که منم نمیدونم چه خطاییه، بهتره لاگ هارو بررسی کنی و در صفحه گیتهاب این مشکل رو گزارش بدی"},
            {WrongCaptcha, "دوباره با دقت تلاش کن :)"},
            {CaptchaStoreError, "دسترسی های پوشه بوستان رو بررسی کنید و برنامه رو دوباره اجرا کنید"},
            {SettingsError, "دسترسی های فایل تنظیمات که در پوشه برنامه قرار داره رو بررسی کنید"},
            {ExtractError, "یکبار دیگه تلاش کن تا بوستان این مشکل رو برات حل کنه"},
            // this is built-in Golestan error codes that we might see.
            {1, "دوباره با دقت تلاش کن :)"},
            {24, "دوباره برای ورود تلاش کن تا این مشکل رفع بشه. اگر نشد لطفا در گیتهاب خبر بده"},
            {27, "یکبار دیگه تلاش کن تا بوستان این مشکل رو برات حل کنه"}
        };
    };
};

#endif // CONSTANTS_H
