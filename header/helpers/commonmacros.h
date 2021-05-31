#ifndef COMMONMACROS_H
#define COMMONMACROS_H

#include <qglobal.h>
#include <QString>

#if defined (Q_OS_WINDOWS)
    #define MyStringLiteral(str) \
            str
#elif defined (Q_OS_LINUX)
    #define MyStringLiteral(str) \
            QStringLiteral(str)
#endif

#endif // COMMONMACROS_H
