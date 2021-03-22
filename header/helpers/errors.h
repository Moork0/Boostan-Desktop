#ifndef ERRORS_H
#define ERRORS_H

/*
    * Class: Errors
    * Files: errors.h, errors.cpp
    * This class act as an interface for showing errors.
    * Convert raw error codes to showing-capable errors and retrieve error titles and description-
    * are the tasks of this class.
*/

#include <QObject>
#include <QNetworkReply>

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
//    Q_PROPERTY(int      errorCode       READ getErrorCode       WRITE setErrorCode  NOTIFY errorCodeChanged)
//    // actually returns 'error_type'
//    Q_PROPERTY(uint     criticalStatus  READ getCriticalStatus  NOTIFY errorCodeChanged)
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
        {24, SemiCritical},
        {27, SemiCritical},
        {18, Normal}        // Access is limited(access denied actually)
    };

    // Error title for each error code that identifyed so far
    inline static const QHash<uint, QString> error_strings
    {
        {ServerConnenctionError, QStringLiteral("خطا در اتصال به سرور")},
        {UnknownError, QStringLiteral("اوه! یک خطای ناشناخته رخ داده!")},
        {WrongCaptcha, QStringLiteral("کد امنیتی اشتباه وارد شده!")},
        {CaptchaStoreError, QStringLiteral("مشکلی در ذخیره تصویر امنیتی بوجود اومده!")},
        {SettingsError, QStringLiteral("مشکلی در تنظیمات بوستان بوجود اومده!")},
        {ExtractError, QStringLiteral("مشکلی در استخراج اطلاعات از گلستان بوجود اومده!")},
        // this is built-in Golestan error codes that we might see.
        {1, QStringLiteral("نام کاربری یا رمز عبوری که وارد شده اشتباهه!")},
        {24, QStringLiteral("گلستان میگه دوباره باید وارد بشی!")},
        {27, QStringLiteral("تعداد تلاش ناموفق برای ورود بیش از حد مجاز شده!")},
        {18, QStringLiteral("امکان دسترسی به محتوای مورد نظر محدود شده!")}
    };

    // Error description for each error code that identifyed so far
    inline static const QHash<uint, QString> error_solutions
    {
        {ServerConnenctionError, QStringLiteral("لطفا وضعیت اتصال به اینترنت و وبسایت گلستان رو بررسی کنید")},
        {UnknownError, QStringLiteral("از اونجایی که منم نمیدونم چه خطاییه، بهتره لاگ هارو بررسی کنی و در صفحه گیتهاب این مشکل رو گزارش بدی")},
        {WrongCaptcha, QStringLiteral("دوباره با دقت تلاش کن :)")},
        {CaptchaStoreError, QStringLiteral("دسترسی های پوشه بوستان رو بررسی کنید و برنامه رو دوباره اجرا کنید")},
        {SettingsError, QStringLiteral("دسترسی های فایل تنظیمات که در پوشه برنامه قرار داره رو بررسی کنید")},
        {ExtractError, QStringLiteral("یکبار دیگه تلاش کن تا بوستان این مشکل رو برات حل کنه")},
        // this is built-in Golestan error codes that we might see.
        {1, QStringLiteral("دوباره با دقت تلاش کن :)")},
        {24, QStringLiteral("دوباره برای ورود تلاش کن تا این مشکل رفع بشه. اگر نشد لطفا در گیتهاب خبر بده")},
        {27, QStringLiteral("یکبار دیگه تلاش کن تا بوستان این مشکل رو برات حل کنه")},
        {18, QStringLiteral("از خودِ سایت گلستان چک کن ببین آیا همه چیز درسته؟ اگر درست بود، این مشکل از بوستانه پس لطفا در گیتهاب به ما گزارش بده.")}
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
        {WrongCaptcha, QStringLiteral("كد امنيتي")}     // error strings that has this keyword are about WrongCaptcha
    };

    explicit            Errors(QObject *parent = nullptr);

    // Set 'value' as error code
    bool                setErrorCode(int ecode);
    // return error code
    int                 getErrorCode()      const;
    // return type of a 'error_code' which is a 'error_type' member
    int                 getErrorType() const;

    // Assign an error type to an error code(specifying the critical-ness of an error)
    void                setCriticalStatus(const int ecode, const Errors::error_type type);
    // reset error_code to NoError
    void                 reset();

//public slots:
    // return a error title for 'error_code'
    QString              getErrorString()    const;
    // return a error description for 'error_code'
    QString              getErrorSolution()  const;

//signals:
//    void                        errorCodeChanged();
};

#endif // ERRORS_H
