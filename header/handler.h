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

#include "network.h"
#include "constants.h"
#include "settings.h"

class Handler : public QObject
{
    Q_OBJECT
private:
    /**  Properties **/

    /* Patterns for extracting necessary data from a response */
    const QString viewstate_pattern         {"__VIEWSTATE\" value=\"\\S+"};                     // regex patern for view state
    const QString viewstate_gen_pattern     {"__VIEWSTATEGENERATOR\" value=\"[A-Za-z0-9]+"};   // regex pattern for viewstate generator
    const QString event_val_pattern         {"__EVENTVALIDATION\" value=\"\\S+"};             // regex pattern for event validation
    const QString tokens_pattern            {"SavAut\\([,a-z0-9'A-Z-]+"};                    // regex pattern for lt,ctck,u,ft,seq and ...
    const QString tck_pattern               {"SetOpenerTck\\('[A-Z0-9-]{16}"};              // regex pattern for tck.
    const QString error_withcode_pattern    {"ErrorArr = new Array\\('[\\w :]+"};          // regex pattern for finding error which has a code

    /** Functions **/

    /*
        * Extract toknes(not validators) from a response and return QHashString
        * which includes token titles as key and token values as value of QHash
    */
    QHashString extractTokens(const QString& response);

    /*
        * Extract error code from response which returned by Golestan
        * return Constants::Errors::NoCodeFound if no code can be found in response.
     */
    int extractDataErrorCode(const QString& response);

    /*
        * Extract error from response.
        * return Constants::Errors::NoError if there is no error
        * return Constants::Errors::UnknownError if there is error
        * but can't find error code in custom errors
     */
    int extractDataError(const QString& response);

protected:
    /**  Properties **/

    Q_PROPERTY(bool     finished        READ getFinished        NOTIFY finished)
    Q_PROPERTY(bool     success         READ getSuccess         NOTIFY successChanged)
    //! TODO: try to change uint to Constants::Errors:error_codes
    Q_PROPERTY(uint     errorCode       READ getErrorCode       NOTIFY errorCodeChanged)
    Q_PROPERTY(bool     working         READ getWorking         NOTIFY workingChanged)

    inline static QHashString      cookies, request_validators;
    //! TODO: try to remove const and add an inline expression
    static const  QString          root_url;

    Network                 request;
    bool                    is_finished, success;
    int                     error_code;

    /** Functions **/

    // Add to cookies a cookie with name of 'key' and value of 'value'
    void        setCookie(QString& key, QString& value);
    /*
        * Add to cookies a cookie in a single string format like this:
        * SomeCookieName=SomeValue
     */
    void        setCookie(QString& keyvalue);
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
        *
        * This function Verify a response by
        * 1- check if there is any error in 'reply' and 'data'
        * 2- update tokens by parsing 'data'
        * return ture if succeed and error_code will be Constants::Errors::NoError
        * otherwise return false and error_code will be a raw error code.
     */
    bool        verifyResponse(QNetworkReply& reply, QString& data);

    // extract form validators from a 'response' and return an empty QHashString if nothing found
    QHashString extractFormValidators(const QString& response);

public:
    explicit    Handler(QObject *parent = nullptr);

signals:
    // determines that a jobs has finished
    void finished();
    void successChanged();
    void errorCodeChanged();
    void workingChanged();
};

#endif // HANDLER_H
