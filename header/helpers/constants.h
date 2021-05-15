#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
    * file name: Constatns.h
    * This file suppose to store all of the constant values which other files might use.
*/

#include <QDir>

using QHashString = QHash <QString, QString>;

namespace Constants {


    inline const QString    application_name{QStringLiteral("Boostan")},
                            organization_name{QStringLiteral("AVID")},
                            domain_name{QStringLiteral("SeedPuller.ir")},
                            application_path {QDir::currentPath() + "/"},
                            // default golestan url for inserting to settings if no configuration is available
                            root_url {QStringLiteral("https://golestan.umz.ac.ir")},
                            settings_path {application_path + QStringLiteral("settings.ini")};

};  // namespace Constants

#endif // CONSTANTS_H
