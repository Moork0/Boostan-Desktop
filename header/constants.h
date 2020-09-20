#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QSettings>
#include <QString>
#include <QVariant>
#include <QDir>
#include <array>

class Constants {

private:
    static QSettings settings;

public:
    const static QString application_name,
                        organization_name,
                        domain_name,
                        application_path;

    Constants() = delete ;
    static QVariant getConstant(const QString& key);
    static void setConstant(const QString& key, const QString& value);

    class Errors
    {
    public:
        enum {
            NoCodeFound             = -1,
            CustomCode              = 500,
            UnknownError            = 1000,
            NoError                 = 0,
            ServerConnenctionError  = CustomCode -1
        };

        Errors() = delete;
        const static QHash<uint, QString> error_strings;
        const static QHash<uint, QString> error_solutions;
    };
};

#endif // CONSTANTS_H
