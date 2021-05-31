#ifndef ERRORS_H
#define ERRORS_H

/*
    * Class: Errors
    * Files: errors.h, errors.cpp
    * This class act as an interface for showing errors.
    * Convert raw error codes to showing-capable errors, retrieving error titles and description-
    * and storing error states are the tasks of this class.
*/

#include <QObject>
#include <QNetworkReply>
#include "commonmacros.h"

class Errors : public QObject
{
    Q_OBJECT
public:
    // our custom error codes.
    enum error_codes
    {
        NoError                 = 0,
        CustomCode              = 800,
        UnknownError            = 1000,
        ServerConnenctionError  = CustomCode,
        WrongCaptcha,
        CaptchaStoreError,
        SettingsError,
        ExtractError
    };

    // error types that identify if a error code is critical or not
    enum error_type: uint
    {
        Critical = 0,
        SemiCritical,
        Normal
    };
    Q_ENUM(error_type)

private:
    int     error_code;

    // type of each error code
    QHash<int, int> critical_status
    {
        {ServerConnenctionError, SemiCritical},
        {UnknownError, Critical},
        {WrongCaptcha, Normal},
        {CaptchaStoreError, Critical},
        {SettingsError, Critical},
        {ExtractError, Normal},
        // this is built-in Golestan error codes that we might see.
        {1, Normal},

        // start of account settings errors
        {3, Normal},
        {4, Normal},
        {6, Normal},
        {8, Normal},
        // end of account settings errors

        {24, Critical},     // authorization destroyed.
        {27, SemiCritical},
        {18, Normal}        // Access is limited(access denied actually)
    };

    // Error title for each error code that identifyed so far
    inline static const QHash<uint, QString> error_strings
    {
        {ServerConnenctionError, MyStringLiteral("خطا در اتصال به سرور")},
        {UnknownError, MyStringLiteral("اوه! یک خطای ناشناخته رخ داده!")},
        {WrongCaptcha, MyStringLiteral("کد امنیتی اشتباه وارد شده!")},
        {CaptchaStoreError, MyStringLiteral("مشکلی در ذخیره تصویر امنیتی بوجود اومده!")},
        {SettingsError, MyStringLiteral("مشکلی در تنظیمات بوستان بوجود اومده!")},
        {ExtractError, MyStringLiteral("مشکلی در استخراج اطلاعات از گلستان بوجود اومده!")},
        // this is built-in Golestan error codes that we might see.
        {1, MyStringLiteral("نام کاربری یا رمز عبوری که وارد شده اشتباهه!")},
        {3, MyStringLiteral("نام کاربری فعلی‌ت رو اشتباه وارد کردی!")},
        {4, MyStringLiteral("رمز فعلی‌ت رو اشتباه وارد کردی!")},
        {6, MyStringLiteral("رمز های جدید باهمدیگه مطابقت ندارن!")},
        {8, MyStringLiteral("نام کاربری ای که وارد کردی قبلا انتخاب شده!")},
        {24, MyStringLiteral("گلستان میگه دوباره باید وارد بشی!")},
        {27, MyStringLiteral("تعداد تلاش ناموفق برای ورود بیش از حد مجاز شده!")},
        {18, MyStringLiteral("امکان دسترسی به محتوای مورد نظر محدود شده!")}
    };

    // Error description for each error code that identifyed so far
    inline static const QHash<uint, QString> error_solutions
    {
        {ServerConnenctionError, MyStringLiteral("لطفا وضعیت اتصال به اینترنت و وبسایت گلستان رو بررسی کنید")},
        {UnknownError, MyStringLiteral("از اونجایی که منم نمیدونم چه خطاییه، بهتره لاگ هارو بررسی کنی و در صفحه گیتهاب این مشکل رو گزارش بدی")},
        {WrongCaptcha, MyStringLiteral("دوباره با دقت تلاش کن :)")},
        {CaptchaStoreError, MyStringLiteral("دسترسی های پوشه بوستان رو بررسی کنید و برنامه رو دوباره اجرا کنید")},
        {SettingsError, MyStringLiteral("دسترسی های فایل تنظیمات که در پوشه برنامه قرار داره رو بررسی کنید")},
        {ExtractError, MyStringLiteral("یکبار دیگه تلاش کن تا بوستان این مشکل رو برات حل کنه")},
        // this is built-in Golestan error codes that we might see.
        {1, MyStringLiteral("دوباره با دقت تلاش کن :)")},
        {3, MyStringLiteral("دوباره با دقت تلاش کن :)")},
        {4, MyStringLiteral("دوباره با دقت تلاش کن :)")},
        {6, MyStringLiteral("دوباره با دقت تلاش کن :)")},
        {8, MyStringLiteral("یک نام کاربری دیگه رو انتخاب کن :)")},
        {24, MyStringLiteral("دوباره برای ورود تلاش کن تا این مشکل رفع بشه. اگر نشد لطفا در گیتهاب خبر بده")},
        {27, MyStringLiteral("یکبار دیگه تلاش کن تا بوستان این مشکل رو برات حل کنه")},
        {18, MyStringLiteral("از خودِ سایت گلستان چک کن ببین آیا همه چیز درسته؟ اگر درست بود، این مشکل از بوستانه پس لطفا در گیتهاب به ما گزارش بده.")}
    };

public:
    /*
        * 'qt_offset' is offset for QNetworkReply::NetworkError codes
        * the reason i defined this is Golestan error codes are in conflict with Qt network error codes.
    */
    inline static constexpr int qt_offset{100};
    inline static constexpr int NoCodeFound{-1};

    /*
        * Key words for identifying an error code for strings that returned by Golestan
        * and has no built-in error codes in it.
    */
    inline static const QHash<int, QString> error_keywords
    {
        {WrongCaptcha, MyStringLiteral("كد امنيتي")}     // error strings that has this keyword are about WrongCaptcha
    };

    explicit            Errors(QObject *parent = nullptr);

    // return error code
    int                 getErrorCode()      const;

    // Assign an error type to an error code(specifying the critical-ness of an error)
    void                setCriticalStatus(const int ecode, const Errors::error_type type);
    // reset error_code to NoError
    void                 reset();

public slots:
    /*
     * We need these functions to accessed directly via QML
     */

    // Set 'ecode' as error code
    bool                setErrorCode(int ecode);

    // return type of a 'error_code' which is a 'error_type' member
    int                 getErrorType() const;

    // return a error title for 'error_code'
    QString              getErrorString()    const;
    // return a error description for 'error_code'
    QString              getErrorSolution()  const;

};

#endif // ERRORS_H
