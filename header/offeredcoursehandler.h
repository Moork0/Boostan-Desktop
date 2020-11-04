#ifndef OFFEREDCOURSEHANDLER_H
#define OFFEREDCOURSEHANDLER_H

#include "handler.h"
#include <QXmlStreamReader>

class OfferedCourseHandler : public Handler
{
    Q_OBJECT

private:
    const QString offered_course_pattern    {"<Root>[\\W\\w]+<\\/Root>"};

    void extractOfferedCourses(const QString& response);

public:
    OfferedCourseHandler();
};

#endif // OFFEREDCOURSEHANDLER_H
