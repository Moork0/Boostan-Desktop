#ifndef OFFEREDCOURSEHANDLER_H
#define OFFEREDCOURSEHANDLER_H

#include "abstractxmldatahandler.h"
#include "../models/offeredcoursemodel.h"
#include <QStringBuilder>

class OfferedCourseHandler : public AbstractXmlDataHandler
{
    Q_OBJECT

private:
    QHash<QString, QVariantList*> container;
    bool    getIsEmpty() const override;
    void    cleanUp();
    void    normalizeTime(QString& time);

public:
    OfferedCourseHandler();
    virtual ~OfferedCourseHandler();
    bool extractOfferedCourses(const QString& response);

public slots:
    void start();
    void sendDataTo(QObject* model);
};

#endif // OFFEREDCOURSEHANDLER_H
