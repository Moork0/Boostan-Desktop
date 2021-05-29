#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
    * file name: Constatns.h
    * This file suppose to store all of the constant values which other files might use.
*/

#include <QDir>
#include <QCalendar>
#include <QDate>

using QHashString = QHash <QString, QString>;
namespace Constants {

    static inline QString generateTodayDate ()
    {
        QLocale locale {QLocale::Persian, QLocale::Iran};
        locale.setNumberOptions(QLocale::OmitGroupSeparator);

        QCalendar calendar(QCalendar::System::Jalali);
        QDate today = QDate::currentDate();
        QCalendar::YearMonthDay ymd = calendar.partsFromDate(today);

        return QString(QStringLiteral("%1 %2 %3"))
                .arg(locale.toString(ymd.day), calendar.monthName(locale, ymd.month), locale.toString(ymd.year));
    }

    static inline const QString application_name{QStringLiteral("Boostan")},
                                organization_name{QStringLiteral("AVID")},
                                domain_name{QStringLiteral("SeedPuller.ir")},
                                application_path {QDir::currentPath() + "/"},
                                // default golestan url for inserting to settings if no configuration is available
                                root_url {QStringLiteral("https://golestan.umz.ac.ir")},
                                settings_path {application_path + QStringLiteral("settings.ini")},
                                today_date {generateTodayDate()},
                                version {"0.1.2"};

};  // namespace Constants

#endif // CONSTANTS_H
