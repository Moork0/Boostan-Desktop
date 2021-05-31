#ifndef LOGGER_H
#define LOGGER_H
/*
    * Class: Logger
    * Files: logger.h, logger.cpp
    * This class will write logs into the disk in proper format
*/

#include <QFile>

class Logger
{

private:
    static inline QString   _file_name      {"boostan.log"};
    static inline QFile      _file          {_file_name};

public:
    Logger() = default;
    // initialize the logger.
    static bool init    ();
    // write logs into the disk. {{more}} defines if we should close the file or not.
    static void log     (const QByteArray& data, const bool more = false);

};

#endif // LOGGER_H
