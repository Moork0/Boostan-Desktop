#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>

class Logger
{

private:    
    QString _file_name;
    QFile _file;
public:
    Logger();
    ~Logger();
    bool init();
    void log(const QByteArray& data);

};

#endif // LOGGER_H
