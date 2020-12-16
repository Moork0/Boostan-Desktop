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
    std::vector<std::vector<QVariant>*>* container;
    OfferedCourseModel view_model;
    bool    getIsEmpty() const override;

public:
    OfferedCourseHandler();
    ~OfferedCourseHandler();
    bool extractOfferedCourses(const QString& response);
};

#endif // OFFEREDCOURSEHANDLER_H
