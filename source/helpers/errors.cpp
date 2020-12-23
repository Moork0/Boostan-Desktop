#include "header/helpers/errors.h"

Errors::Errors(QObject *parent) : QObject(parent), error_code{0}
{

}

QString Errors::getErrorString() const
{
    return Constants::Errors::error_strings[error_code];
}

QString Errors::getErrorSolution() const
{
    return Constants::Errors::error_solutions[error_code];
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
    if (ecode >= (QNetworkReply::ConnectionRefusedError + Constants::Errors::qt_offset) && ecode <= (QNetworkReply::UnknownServerError + Constants::Errors::qt_offset))
        error_code = Constants::Errors::ServerConnenctionError;

    // is this error code discovered befor? if not, we can't do anything about it.
    else if (!Constants::Errors::error_strings.contains(ecode))
        error_code = Constants::Errors::UnknownError;

    else
        error_code = ecode;

    emit errorCodeChanged();
}

// returns the type of error
uint Errors::getCriticalStatus() const
{
    return Constants::Errors::critical_status.value(error_code, Constants::Errors::Normal);
}

void Errors::reset()
{
    setErrorCode(Constants::Errors::NoError);
}
