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
     QList <QVariantList*> container;
     const QHash<QString, QVariantMap> schedule;

    bool    getIsEmpty() const override;
    void    cleanUp();
    void    normalizeTime(QString& time);
    bool    CheckIsChoosed (const QString& key, const QHash<QString, QVariantMap>& schedule) const;

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
