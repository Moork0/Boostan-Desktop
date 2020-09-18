#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include <QObject>
#include <QRegularExpression>
#include <QXmlStreamReader>
#include <QDebug>

using QHashString = QHash <QString, QString>;

namespace TextParser
{

    const QString viewstate_pattern         {"__VIEWSTATE\" value=\"\\S+"};                     // regex patern for view state
    const QString viewstate_gen_pattern     {"__VIEWSTATEGENERATOR\" value=\"[A-Za-z0-9]+"};   // regex pattern for viewstate generator
    const QString event_val_pattern         {"__EVENTVALIDATION\" value=\"\\S+"};             // regex pattern for event validation
    const QString tokens_pattern            {"SavAut\\([,a-z0-9'A-Z-]+"};                    // regex pattern for lt,ctck,u,ft,seq and ...
    const QString tck_pattern               {"SetOpenerTck\\('[A-Z0-9-]{16}"};              // regex pattern for tck.
    const QString student_name_pattern      {"SetUsr\\('[\\w ,']+"};                       // regex pattern for student name
    const QString offered_course_pattern    {"<Root>[\\W\\w]+<\\/Root>"};

    bool hasError(const QString& response);
    QHashString extractLoginValidators(const QString& response);
    QHashString extractTokens(const QString& response);
    QString extractStudentName(const QString& response);
    void extractOfferedCourses(const QString& response);
};


#endif // TEXTPARSER_H
