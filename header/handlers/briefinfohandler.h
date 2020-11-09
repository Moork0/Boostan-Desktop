#ifndef DASHBOARDHANDLER_H
#define DASHBOARDHANDLER_H

/*
    * Class: BriefInfoHandler
    * Files: briefinfohandler.h, briefinfohandler.cpp
    * The task of this class is to request to the "general information" section of Golestan,
    * parse that information inclues semesters averages and identity informations
    * then create a neat structure for exposing those informations to QML(probably DashboardPage.qml)
*/

#include "handler.h"
#include <vector>

class BriefInfoHandler : public Handler
{
    Q_OBJECT
private:
    /** Properties  **/
    Q_PROPERTY(QVariantMap  briefInfo   READ getStudentInfo     NOTIFY studentInfoChanged)
    Q_PROPERTY(int          currentYear READ getCurrentYear     NOTIFY currentYearChanged)

    inline static QString       user_info_url {"/Forms/F1802_PROCESS_MNG_STDJAMEHMON/F1802_01_PROCESS_MNG_STDJAMEHMON_Dat.aspx?r=0.9638806400489983&fid=0;12310&b=0&l=0&&lastm=20180201081222&tck="};
    // this is the keys of a QVariantMap that we wanna expose to qml
    const std::vector<QString>  info_title {"id", "field", "studyType", "average", "passedUnits"};
    //! TODO: convert to QVariantMap
    QMap<QString, QString>      student_info;
    QList<int>                  passed_semesters;       // passed semesters
    QStringList                 passed_semesters_avg;   // semesters averages
    QLocale                     locale;
    int                         current_year;           // current semester

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
    //! TODO: change name to extractStuId
    QString         extractStudentNumber(const QString& response);

public:
    // index of each key in info_title
    //! TODO: change INDEX to Index
    enum info_index {
        INDEX_START = -1,   // used to determine the start of enum in loops
        INDEX_Id,
        INDEX_Field,
        INDEX_StudyType,
        INDEX_TotalAvg,
        INDEX_Passed,
        INDEX_END           // used to determine the end of enum in loops
    };


    BriefInfoHandler();
    // return current_year
    int             getCurrentYear() const;

private slots:
    // parse the response to requestBriefInfo()
    void            parseUserInfo(QNetworkReply& reply);
    // parse the response to requestStuId()
    //! TODO: change name to parseStuId()
    void            parseUserNumber(QNetworkReply& reply);
    // parse the response to requestTokens()
    void            parseTokens(QNetworkReply& reply);

public slots:
    // call requestTokens()
    void            start();
    // return passed_semesters_avg
    QStringList     getSemesterAvgs()  const;
    // return passed_semsesters
    QStringList     getSemesterYears() const;

signals:
    void            studentInfoChanged();
    void            currentYearChanged();
};

#endif // DASHBOARDHANDLER_H
