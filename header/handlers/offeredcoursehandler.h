#ifndef OFFEREDCOURSEHANDLER_H
#define OFFEREDCOURSEHANDLER_H

#include "abstractxmldatahandler.h"
#include "../models/offeredcoursemodel.h"
#include <QStringBuilder>
#include <vector>

class OfferedCourseHandler : public AbstractXmlDataHandler
{
    Q_OBJECT

private:
    ContainerType* container;
    bool    getIsEmpty() const override;
    void    cleanUp();

public:
    OfferedCourseHandler();
    virtual ~OfferedCourseHandler();
    bool extractOfferedCourses(const QString& response);

public slots:
    void start();
    void sendDataTo(QObject* model);
};

#endif // OFFEREDCOURSEHANDLER_H
