#ifndef COURSESCHEDULEHANDLER_H
#define COURSESCHEDULEHANDLER_H

/*
    * Class: CourseScheduleHandler
    * Files: courseschedulehandler.h, courseschedulehandler.cpp
    * The task of this class is to get the courses weekly schedule (and related informations)
    * from Golestan and send them to QML in proper structure.
*/

#include "abstractxmldatahandler.h"
#include <QStringBuilder>

class CourseScheduleHandler : public AbstractXmlDataHandler
{
    Q_OBJECT

private:
    /** Properties **/

    const QString   schedule_url          {QStringLiteral("/Forms/F0202_PROCESS_REP_FILTER/F0202_01_PROCESS_REP_FILTER_DAT.ASPX?r=0.7010287827974179&fid=1;73&b=0&l=0&&lastm=20190829142532&tck=")};
    QString         year;
    // our structure for store weekly schedule
    QList<QList<QVariant>> weekly_schedule;
    // number of days of week. also the number of classe's per day is equal to this
    const int week_days {5};

    /** Functions **/

    // return the index of a time of a class in the structure ( for example: 8:00 is the first class
    // (so the index is 0) and 17:00 is the last class)
    int     hourIndex               (QString& hour) const;
    // extract weekly course schedule from 'response' and fill 'weekly_schedule'
    bool    extractWeeklySchedule   (QString& response);
    // extract the current semester from 'response'
    bool    extractCurrentYear      (QString& response);
    // request validators for being able to make further requests
    bool    requestTokens();
    // request the weekly schedule
    bool    requestSchedule();
    // getter for is_empty
    bool    getIsEmpty() const override;

private slots:
    // parse the validators from request requestTokens()
    void    parseTokens             (QNetworkReply& reply);
    // parse the weekly schedule
    void    parseSchedule           (QNetworkReply& reply);

public:
    CourseScheduleHandler();
    // return a schedule for specific day 'day' (0 is Saturday)
    Q_INVOKABLE QVariantList    dailyScheduleModel  (int day) const;
    // start the process for recieving the schedule of 'current' semester
    Q_INVOKABLE void            start               (int current);
};

#endif // COURSESCHEDULEHANDLER_H
