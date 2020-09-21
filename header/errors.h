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
    Q_PROPERTY(bool     isCritical      READ isCritical         NOTIFY errorCodeChanged)
//    Q_PROPERTY(QString  errorString     READ getErrorString     NOTIFY errorCodeChanged)
//    Q_PROPERTY(QString  errorSolution   READ getErrorSolution   NOTIFY errorCodeChanged)
    int     error_code;
//    bool    is_critical;

public:
    explicit                    Errors(QObject *parent = nullptr);

    int                  getErrorCode() const;
    void                        setErrorCode(int value);

    bool                        isCritical() const;
//    inline void                 setCritical(bool state);

public slots:
    void                        reset();
    QString              getErrorString()    const;
    QString              getErrorSolution()  const;

signals:
    void                        errorCodeChanged();
};

#endif // ERRORS_H
