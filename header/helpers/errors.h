#ifndef ERRORS_H
#define ERRORS_H

/*
    * Class: Errors
    * Files: errors.h, errors.cpp
    * This class act as an interface for showing errors.
    * Convert raw error codes to showing-capable errors and retrieve error titles and description
    * are the tasks of this class.
*/

#include <QObject>
#include <QNetworkReply>
#include "constants.h"

class Errors : public QObject
{
    Q_OBJECT
private:
    Q_PROPERTY(int      errorCode       READ getErrorCode       WRITE setErrorCode  NOTIFY errorCodeChanged)
    //! TODO: change return type to error_type
    Q_PROPERTY(uint     criticalStatus  READ getCriticalStatus  NOTIFY errorCodeChanged)
    int     error_code;

public:

    // error type that identify if a error code is critical or ...
    enum error_type {
        Critical        = Constants::Errors::Critical,
        SemiCritical    = Constants::Errors::SemiCritical,
        Normal          = Constants::Errors::Normal
    };
    Q_ENUM(error_type)

    explicit            Errors(QObject *parent = nullptr);

    // Set 'value' as error code
    void                setErrorCode(int ecode);
    // return error code
    int                 getErrorCode()      const;
    // return type of a 'error_code'
    //! TODO: change return type to error_type
    uint                getCriticalStatus() const;

public slots:
    // reset error_code to NoError
    void                 reset();
    // return a error title for 'error_code'
    QString              getErrorString()    const;
    // return a error description for 'error_code'
    QString              getErrorSolution()  const;

signals:
    void                        errorCodeChanged();
};

#endif // ERRORS_H
