#ifndef HANDLER_H
#define HANDLER_H

/*
    * Class: Handler
    * Files: handler.h, handler.cpp
    *
    * This is the primary class that handles every request and every response
    * sent to and recieve from Golestan system.
    * Extract validation values from every response, use validations correctly for every requests
    * and check if a response has any error(and extract possible error code) are the main responsibilities
    * of this class.
    * All the other *Handler classes are derived from this class.
    *
*/

#include <QObject>
#include <QUrl>
#include <QFile>
#include <QHash>
#include <QDebug>
#include <QRegularExpression>

#include "../base/network.h"
#include "../base/settings.h"
#include "../helpers/errors.h"
#include "../helpers/logger.h"

class Handler : public QObject
{
    Q_OBJECT
private:
    /**  Properties **/

    /* Patterns for extracting necessary data from a response */
    const QString _tokens_pattern            {QStringLiteral("SavAut\\([,a-z0-9'A-Z-]+")};                           // regex pattern for lt,ctck,u,ft,seq and ...
    const QString _error_withcode_pattern    {QStringLiteral("ErrorArr = new Array\\('[\\w :]+")};                   // regex pattern for finding error which has a code

    const QString _viewstate_keyword         {QStringLiteral("__VIEWSTATE\" value=\"")};             // keyword for finding view state
    const QString _viewstate_gen_keyword     {QStringLiteral("__VIEWSTATEGENERATOR\" value=\"")};    // keyword for finding viewstate generator
    const QString _event_val_keyword         {QStringLiteral("__EVENTVALIDATION\" value=\"")};       // keyword for finding event validation
    const QString _tck_keyword               {QStringLiteral("SetOpenerTck('")};                     // keyword for finding tck

    /** Functions **/

    /*
        * Extract toknes(not validators) from a response and return QHashString
        * which includes token titles as key and token values as value of QHash
    */
    QHashString extractTokens(const QString& response);

    /*
        * Extract error code from response which returned by Golestan
        * return Errors::NoCodeFound if no code can be found in response.
     */
    int extractDataErrorCode(const QString& response);

    /*
        * Extract error from response.
        * return Errors::NoError if there is no error
        * return Errors::UnknownError if there is error
        * but can't find error code in custom errors
     */
    int extractDataError(const QString& response);

protected:
    /**  Properties **/

    Q_PROPERTY(bool     finished        READ getFinished        NOTIFY finished)
    Q_PROPERTY(bool     success         READ getSuccess         NOTIFY successChanged)
    // returns Golestan errors or Errors:error_code
    Q_PROPERTY(uint     errorCode       READ getErrorCode       NOTIFY errorCodeChanged)
    Q_PROPERTY(int      errorType       READ getErrorType       NOTIFY errorCodeChanged)
    Q_PROPERTY(bool     working         READ getWorking         NOTIFY workingChanged)

    static inline QHashString      _cookies, _request_validators;
    static inline QString          _root_url;

    Network                 _request;
    Errors                  _error_handler;
    bool                    _is_finished, _success;

    /** Functions **/

    // Add to cookies a cookie with name of 'key' and value of 'value'
    void        setCookie(const QString& key, const QString& value);
    /*
        * Add to cookies a cookie in a single string format like this:
        * SomeCookieName=SomeValue
     */
    void        setCookie(const QString& keyvalue);
    // remove all cookies
    void        clearCookies();

    // return cookies in string format that capable of being used as a request header
    QString     getCookies() const;

    // return 'is_finished'
    bool        getFinished() const;
    // set 'is_finished' to 'value'
    void        setFinished(bool value);
    // return true if there is any request working
    bool        getWorking() const;

    /*
     * simple setter and getter for error_code
     */
    uint        getErrorCode() const;
    void        setErrorCode(int ecode);

    // Get error type(Critical-ness of a error) from Errors
    int         getErrorType() const;

    /*
     * simple setter and getter for success
     */
    void        setSuccess(bool state);
    bool        getSuccess() const;

    // set 'error_code' to 'ecode' and return true if QNetworkReply::NetworkError == QNetworkReply::NoError
    bool        hasError(QNetworkReply::NetworkError ecode);

    /*
        * extract tokens and update 'cookies' and 'request_validators'
        * return true if succeed otherwise return false
     */
    bool        updateTokens(const QString& data);

    /*
        * This is important function and will be used in almost all of the
        * functions that parse Golestan response.
        * 'data' can have response or be empty. if data == QString(), the function
        * will fill it in proper moment.

        * This function Verify a response by:
        * 1- check if there is any error in 'reply' and 'data'
        * 2- update tokens by parsing 'data'
        * return ture if succeed and error_code will be Errors::NoError
        * otherwise return false and error_code will be a raw error code.
     */
    bool        verifyResponse(QNetworkReply& reply, QString& data);

    // extract form validators from a 'response' and return an empty QHashString if nothing found
    QHashString extractFormValidators(const QString& response);
    // return the tck or ctck token for authentication in Golestan
    QString     getTckToken() const;

public:
    explicit    Handler(QObject *parent = nullptr);

public slots:
    // return a error title for 'error_code' in error_handler
    QString     getErrorString()    const;
    // return a error description for 'error_code' in error_handler
    QString     getErrorSolution()  const;

signals:
    // determines that a jobs has finished
    void finished();
    void successChanged();
    void errorCodeChanged();
    void workingChanged();
};

#endif // HANDLER_H
