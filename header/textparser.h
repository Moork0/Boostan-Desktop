#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include <QObject>
#include <QRegularExpression>
#include <QXmlStreamReader>
#include <QDebug>
#include "constants.h"

/*
 * I think this file should being removed and all of the variables should move to
 * Constants.h because they're somehow a constant.
 * Also all of the functions should move to a relevant class file(handlers probably).
 * This will make that classes bigger but that's better because of one of the SOLID principles
 * which i dont remember that =)
 */
namespace TextParser
{

    namespace Validators
    {
        const QString viewstate_pattern         {"__VIEWSTATE\" value=\"\\S+"};                     // regex patern for view state
        const QString viewstate_gen_pattern     {"__VIEWSTATEGENERATOR\" value=\"[A-Za-z0-9]+"};   // regex pattern for viewstate generator
        const QString event_val_pattern         {"__EVENTVALIDATION\" value=\"\\S+"};             // regex pattern for event validation
        const QString tokens_pattern            {"SavAut\\([,a-z0-9'A-Z-]+"};                    // regex pattern for lt,ctck,u,ft,seq and ...
        const QString tck_pattern               {"SetOpenerTck\\('[A-Z0-9-]{16}"};              // regex pattern for tck.

        QHashString extractFormValidators(const QString& response);
        QHashString extractTokens(const QString& response);
    };

    namespace Errors
    {
        constexpr int NoCodeFound {-1};
        const QString error_withcode_pattern      {"ErrorArr = new Array\\('[\\w :]+"};
        const QHash<int, QString> error_keywords {{Constants::Errors::WrongCaptcha, "كد امنيتي"}};

        int extractErrorCode(const QString& response);
        int hasError(const QString& response);
    };

    const QString offered_course_pattern    {"<Root>[\\W\\w]+<\\/Root>"};

    void extractOfferedCourses(const QString& response);
    QString extractStudentNumber(const QString& response);
};


#endif // TEXTPARSER_H
