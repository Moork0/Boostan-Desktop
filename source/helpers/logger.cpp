#include "header/helpers/logger.h"
#include <QTime>
#include <QDebug>

Logger::Logger()
    : _file_name {"boostan.log"}, _file(_file_name)
{
}

Logger::~Logger()
{
    if (_file.isOpen())
        _file.close();
}

bool Logger::init()
{
    if (!_file.open(QIODevice::WriteOnly)) {
        return false;
    }
    return true;
}

void Logger::log(const QByteArray &data)
{
    _file.write((QStringLiteral("=================\n[") + (QTime::currentTime()).toString() + QStringLiteral("]: ")).toUtf8());
    _file.write(data);
    _file.write("\n=================\n");
}
