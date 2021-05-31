#ifndef COMMONMACROS_H
#define COMMONMACROS_H

/*
    * file name: commonmacros.h
    * This file suppose to store all of the Macro's we might use in our code.
*/

#include <QtGlobal>
#include <QString>

// Just use QStringLiteral when we are not in windows
#if defined (Q_OS_WINDOWS)
    #define MyStringLiteral(str) \
            str
#elif defined (Q_OS_LINUX)
    #define MyStringLiteral(str) \
            QStringLiteral(str)
#elif defined (Q_OS_MACOS)
    #define MyStringLiteral(str) \
            QStringLiteral(str)
#endif

#endif // COMMONMACROS_H
