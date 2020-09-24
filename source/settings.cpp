#include "header/settings.h"

Settings::Settings()
{

}

void Settings::setValue(QString key, QString value)
{
    Constants::setConstant(key, value);
}

QString Settings::getValue(QString key)
{
    return Constants::getConstant(key).toString();
}
