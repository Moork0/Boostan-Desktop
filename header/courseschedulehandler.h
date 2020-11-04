#ifndef COURSESCHEDULEHANDLER_H
#define COURSESCHEDULEHANDLER_H

#include "handler.h"
#include <QXmlStreamReader>

class CourseScheduleHandler : public Handler
{
    Q_OBJECT

private:
    const QString xmldata_pattern    {"<Root>[\\W\\w]+<\\/Root>"};
    QList<QList<QVariant>> weekly_schedule;

    int hourIndex(QString& hour) const;
    void extractWeeklySchedule(QString& response);
    bool requestTokens();
    bool requestWeeklySchedule();

public:
    CourseScheduleHandler();
    Q_INVOKABLE QList<QVariant> DailyScheduleModel(int day) const;

private slots:
    void parseTokens(QNetworkReply& reply);
    void parseWeeklySchedule(QNetworkReply& reply);

public slots:
    void start();
};

#endif // COURSESCHEDULEHANDLER_H
