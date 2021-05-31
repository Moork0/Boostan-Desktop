#include "header/helpers/logger.h"
#include <QTime>
#include <QDebug>

bool Logger::init()
{
    if (!_file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    return true;
}

void Logger::log(const QByteArray &data, const bool more)
{
    if (!_file.isOpen() && !_file.open(QIODevice::Append))
        return;

    _file.write((QStringLiteral("=================\n[") + (QTime::currentTime()).toString() + QStringLiteral("]: ")).toUtf8());
    _file.write(data);
    _file.write("\n=================\n");

    if (!more)
        _file.close();
}
