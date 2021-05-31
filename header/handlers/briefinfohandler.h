#ifndef DASHBOARDHANDLER_H
#define DASHBOARDHANDLER_H

/*
    * Class: BriefInfoHandler
    * Files: briefinfohandler.h, briefinfohandler.cpp
    * The task of this class is to request to the "general information" section of Golestan,
    * parse that information which includes semesters averages and identity informations
    * then create a neat structure for exposing those informations to QML
*/

#include "handler.h"
#include <QStringBuilder>
#include <vector>

class BriefInfoHandler : public Handler
{
    Q_OBJECT
private:
    /** Properties  **/
    Q_PROPERTY(QVariantMap  briefInfo   READ getStudentInfo     NOTIFY studentInfoChanged)
    Q_PROPERTY(int          currentYear READ getCurrentYear     NOTIFY currentYearChanged)

    // url of section we gonna send our requests.
    static inline QString       _user_info_url {QStringLiteral("/Forms/F1802_PROCESS_MNG_STDJAMEHMON/F1802_01_PROCESS_MNG_STDJAMEHMON_Dat.aspx?r=0.9638806400489983&fid=0;12310&b=0&l=0&&lastm=20180201081222&tck=")};

    // this is the keys of a QVariantMap that we wanna expose to qml
    /// WHY I CHOOSED STD::VECTOR ??? :/
    const std::vector<QString>  _info_title {QStringLiteral("id"), QStringLiteral("field"), QStringLiteral("studyType"), QStringLiteral("average"), QStringLiteral("passedUnits")};

    // Container which our information would stored in.
    QVariantMap                 _student_info;
    QList<int>                  _passed_semesters;       // passed semesters
    QStringList                 _passed_semesters_avg;   // semesters averages
    QLocale                     _locale;
    int                         _current_year;           // current semester

    /** Functions **/

    // request validators
    bool            requestTokens();
    // request student id
    bool            requestStuId();
    // request student information brief
    bool            requestBriefInfo();

    /*
        * return student general information as a QVariantMap
        *  with the keys 'info_title' and related values for using in QML
    */
    QVariantMap     getStudentInfo() const;
    // extract general information from 'response' and fill 'student_info'
    bool            extractStudentInfo  (const QString& response);
    // extract semester averages and fill 'passed_semesters' and 'passed_semester_avg'
    bool            extractStudentAvgs  (const QString& response);
    // extract student id from 'response' and fill related index in student_info
    QString         extractStuId(const QString& response);

public:
    // index of each key in info_title
    enum info_index {
        Index_START = -1,   // used to determine the start of enum in loops
        Index_Id,
        Index_Field,
        Index_StudyType,
        Index_TotalAvg,
        Index_Passed,
        Index_END           // used to determine the end of enum in loops
    };


    BriefInfoHandler();
    // return current_year
    int             getCurrentYear() const;

private slots:
    // parse the response to requestBriefInfo()
    void            parseUserInfo(QNetworkReply& reply);
    // parse the response to requestStuId()
    void            parseStuId(QNetworkReply& reply);
    // parse the response to requestTokens()
    void            parseTokens(QNetworkReply& reply);

public slots:
    // call requestTokens()
    void            start();
    // return passed_semesters_avg
    QStringList     getSemesterAvgs()  const;
    // return passed_semesesters in readable format
    QStringList     getSemesterYears() const;
    // return passed_semesters
    QList<int>      getRawSemesters() const;

signals:
    void            studentInfoChanged();
    void            currentYearChanged();
};

#endif // DASHBOARDHANDLER_H
