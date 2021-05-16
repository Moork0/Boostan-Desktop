#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>

class Logger
{

private:
    static inline QString   _file_name      {"boostan.log"};
    static inline QFile      _file          {_file_name};

public:
    Logger() = default;
    static bool init    ();
    static void log     (const QByteArray& data, const bool more = false);

};

#endif // LOGGER_H
