#ifndef OFFEREDCOURSEHANDLER_H
#define OFFEREDCOURSEHANDLER_H

#include "abstractxmldatahandler.h"
#include "../models/offeredcoursemodel.h"
#include "../controls/scheduletable.h"
#include <QStringBuilder>

class OfferedCourseHandler : public AbstractXmlDataHandler
{
    Q_OBJECT

private:
    const QString offered_course_url {QStringLiteral("/Forms/F0202_PROCESS_REP_FILTER/F0202_01_PROCESS_REP_FILTER_DAT.ASPX?r=0.6914703770312649&fid=1;211&b=0&l=0&isb=4&lastm=20190829142532&tck=")};

    QList <QVariantList*> container;
    const QHash<QString, QVariantMap> schedule;

    bool    getIsEmpty() const override;
    void    cleanUp();
    void    normalizeTime(QString& time);
    bool    CheckIsChoosed (const QString& key, const QHash<QString, QVariantMap>& schedule) const;
    void    parseRequest(QNetworkReply& reply);

public:
    OfferedCourseHandler();
    virtual ~OfferedCourseHandler();
    bool extractOfferedCourses(const QString& response);

public slots:
    void start();
    void sendDataTo(QObject* model);
    QVariantList restoreSchedule() const;
};

#endif // OFFEREDCOURSEHANDLER_H
