#include "header/helpers/errors.h"

Errors::Errors(QObject *parent) : QObject(parent), error_code{0}
{

}

QString Errors::getErrorString() const
{
    return error_strings[error_code];
}

QString Errors::getErrorSolution() const
{
    return error_solutions[error_code];
}

int Errors::getErrorCode() const
{
    return error_code;
}

// set 'error_code' to proper code by parsing 'ecode'
void Errors::setErrorCode(int ecode)
{
    if (error_code == ecode) return;
    // check if ecode is one of QNetworkReply::Error's then mark them all as ServerConnectionError
    if (ecode >= (QNetworkReply::ConnectionRefusedError + qt_offset) && ecode <= (QNetworkReply::UnknownServerError + qt_offset))
        error_code = ServerConnenctionError;

    // is this error code discovered befor? if not, we can't do anything about it.
    else if (!error_strings.contains(ecode))
        error_code = UnknownError;

    else
        error_code = ecode;

    emit errorCodeChanged();
}

// returns the type of error
uint Errors::getCriticalStatus() const
{
    return critical_status.value(error_code, Normal);
}

void Errors::setCriticalStatus(const Errors::error_codes ecode, const Errors::error_type type)
{
    critical_status[ecode] = type;
}

void Errors::reset()
{
    setErrorCode(NoError);
}
