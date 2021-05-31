#ifndef OFFEREDCOURSEHANDLER_H
#define OFFEREDCOURSEHANDLER_H

/*
    * Class: OfferedCourseHandler
    * Files: offeredcoursehandler.h, offeredcoursehandler.cpp
    * The task of this class is:
    * 1- Retrieve offered course from Golestan
    * 2- Parse and extract necessary information
    * 3- Move the extracted data to OfferedCourseModel
    * 4- Restore the ScheduleTable(Weekly schedule created by user) from disk and integrate
    * the schedule with the data
*/

#include "abstractxmldatahandler.h"
#include "../models/offeredcoursemodel.h"
#include "../controls/scheduletable.h"
#include <QStringBuilder>

class OfferedCourseHandler : public AbstractXmlDataHandler
{
    Q_OBJECT

private:
    const QString _offered_course_url {QStringLiteral("/Forms/F0202_PROCESS_REP_FILTER/F0202_01_PROCESS_REP_FILTER_DAT.ASPX?r=0.6914703770312649&fid=1;%1&b=0&l=0&isb=4&lastm=20190829142532&tck=")};
    // we have 2 pages with information of offered course. their 'fid' parameter is below:
    const QStringList _url_fids {QStringLiteral("212"), QStringLiteral("211")};

    // our container for storing offered course data
    QList <QVariantList*> _container;
    // our container for restoring schedule table from disk
    const QHash<QString, QVariantMap> _schedule;
    // indicate how many we tried for retrieving data from Golestan.
    int     _request_number;

    // Forced implementation of AbstractXmlDataHandler pure function
    bool    getIsEmpty              () const override;
    // clear the containers and free up the memory
    void    cleanUp();
    // Replace wrong characters in 'time' and simplify() the 'time'
    void    normalizeTime           (QString& time);
    // Check if a course with key 'key' is already choosed(and present in 'schedule') or not.
    bool    CheckIsChoosed          (const QString& key, const QHash<QString, QVariantMap>& schedule) const;
    // request for offerd course information
    void    requestCourses          ();
    // Extract needed information from 'response'
    bool    extractOfferedCourses   (const QString& response);

public:
    OfferedCourseHandler            ();
    virtual ~OfferedCourseHandler   ();

private slots:
    // parse the response from requestCourse()
    void    parseCourses            (QNetworkReply& reply);

public slots:
    void    start();
    // move 'container' to the 'model' which in this case is OfferedCourseModel
    void    sendDataTo              (QObject* model);
    // Restore the ScheduleTable data that stored in disk.
    QVariantList restoreSchedule    () const;

};

#endif // OFFEREDCOURSEHANDLER_H
