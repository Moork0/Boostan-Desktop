#ifndef COURSESCHEDULEHANDLER_H
#define COURSESCHEDULEHANDLER_H

#include "handler.h"
#include <QXmlStreamReader>

class CourseScheduleHandler : public Handler
{
    Q_OBJECT

private:
    const QString   schedule_url          {"/Forms/F0202_PROCESS_REP_FILTER/F0202_01_PROCESS_REP_FILTER_DAT.ASPX?r=0.7010287827974179&fid=1;73&b=0&l=0&&lastm=20190829142532&tck="};
    const QString   xmldata_pattern       {"<Root>[\\W\\w]+<\\/Root>"};
    QString         year;
    QList<QList<QVariant>> weekly_schedule;

    int     hourIndex               (QString& hour) const;
    bool    extractWeeklySchedule   (QString& response);
    bool    extractCurrentYear      (QString& response);
    bool    requestTokens();
    bool    requestSchedule();

private slots:
    void    parseTokens             (QNetworkReply& reply);
    void    parseSchedule           (QNetworkReply& reply);

public:
    CourseScheduleHandler();
    Q_INVOKABLE QVariantList    dailyScheduleModel  (int day) const;
    Q_INVOKABLE void            start               (int current);
};

#endif // COURSESCHEDULEHANDLER_H
