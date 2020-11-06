#ifndef DASHBOARDHANDLER_H
#define DASHBOARDHANDLER_H

#include "handler.h"
#include <vector>

class BriefInfoHandler : public Handler
{
    Q_OBJECT
private:
    Q_PROPERTY(QVariantMap  briefInfo   READ getStudentInfo     NOTIFY studentInfoChanged)
    Q_PROPERTY(int          currentYear READ getCurrentYear     NOTIFY currentYearChanged)

    inline static QString       user_info_url {"/Forms/F1802_PROCESS_MNG_STDJAMEHMON/F1802_01_PROCESS_MNG_STDJAMEHMON_Dat.aspx?r=0.9638806400489983&fid=0;12310&b=0&l=0&&lastm=20180201081222&tck="};
    const std::vector<QString>  info_title {"id", "name", "field", "studyType", "average", "passedUnits"};
    QMap<QString, QString>      student_info;
    QList<int>                  passed_semesters;
    QStringList                 passed_semesters_avg;
    QLocale                     locale;
    int                         current_year;

    bool            requestTokens();
    bool            requestStuId();
    bool            requestBriefInfo();
    QVariantMap     getStudentInfo() const;
    bool            extractStudentInfo  (const QString& response);
    bool            extractStudentAvgs  (const QString& response);
    QString         extractStudentNumber(const QString& response);

public:
    enum info_index {
        INDEX_START = -1,   // used to determine the start of enum in loops
        INDEX_Id,
        INDEX_Name,
        INDEX_Field,
        INDEX_StudyType,
        INDEX_TotalAvg,
        INDEX_Passed,
        INDEX_END           // used to determine the end of enum in loops
    };


    BriefInfoHandler();
    int             getCurrentYear() const;

private slots:
    void            parseUserInfo(QNetworkReply& reply);
    void            parseUserNumber(QNetworkReply& reply);
    void            parseTokens(QNetworkReply& reply);

public slots:
    void            start();
    QStringList     getSemesterAvgs()  const;
    QStringList     getSemesterYears() const;

signals:
    void            studentInfoChanged();
    void            currentYearChanged();
};

#endif // DASHBOARDHANDLER_H
