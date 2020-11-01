#ifndef DASHBOARDHANDLER_H
#define DASHBOARDHANDLER_H

#include "handler.h"

class DashboardHandler : public Handler
{
    Q_OBJECT
private:
    inline static QString user_info_url {"/Forms/F1802_PROCESS_MNG_STDJAMEHMON/F1802_01_PROCESS_MNG_STDJAMEHMON_Dat.aspx?r=0.9638806400489983&fid=0;12310&b=0&l=0&&lastm=20180201081222&tck="};
    QStringList student_info;
    QList<QPair<int, float>> student_avgs;

    bool getTokens();
    bool getUserNumber();
    bool getUserInfo();
    bool extractStudentInfo(const QString& response);
    bool extractStudentAvgs(const QString& response);
public:
    enum user_info {
        Number,
        Name,
        Field,
        StudyType,
        TotalAvg,
        Passed
    };
    Q_ENUM(user_info);

    DashboardHandler();

private slots:
    void parseUserInfo(QNetworkReply& reply);
    void parseUserNumber(QNetworkReply& reply);
    void parseTokens(QNetworkReply& reply);

public slots:
    void start();
};

#endif // DASHBOARDHANDLER_H
