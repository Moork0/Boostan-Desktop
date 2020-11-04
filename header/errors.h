#ifndef ERRORS_H
#define ERRORS_H

#include <QObject>
#include <QNetworkReply>
#include "constants.h"

class Errors : public QObject
{
    Q_OBJECT
private:
    Q_PROPERTY(int      errorCode       READ getErrorCode       WRITE setErrorCode  NOTIFY errorCodeChanged)
    Q_PROPERTY(uint     criticalStatus  READ getCriticalStatus  NOTIFY errorCodeChanged)
    int     error_code;

public:

    enum error_type {
        Critical        = Constants::Errors::Critical,
        SemiCritical    = Constants::Errors::SemiCritical,
        Normal          = Constants::Errors::Normal
    };
    Q_ENUM(error_type)

    explicit            Errors(QObject *parent = nullptr);
    void                setErrorCode(int value);
    int                 getErrorCode()      const;
    uint                getCriticalStatus() const;

public slots:
    void                 reset();
    QString              getErrorString()    const;
    QString              getErrorSolution()  const;

signals:
    void                        errorCodeChanged();
};

#endif // ERRORS_H
