#include "header/handlers/offeredcoursehandler.h"

OfferedCourseHandler::OfferedCourseHandler()
{

}


void OfferedCourseHandler::extractOfferedCourses(const QString& response)
{
//    QRegularExpression re {offered_course_pattern, QRegularExpression::UseUnicodePropertiesOption};
//    QRegularExpressionMatch match {re.match(response)};
//    if (match.hasMatch()) {
//        QFile f("courses.xml");
//        f.open(QIODevice::WriteOnly);
//        f.write(match.captured().toUtf8());
//        f.close();
//    }

    QXmlStreamReader reader(response);
    if (reader.readNextStartElement()) {
        if (reader.name() == "Root") {
            while(reader.readNextStartElement()) {
//                Q_ASSERT(reader.isStartElement() && reader.name() != QLatin1String("xbel"));
                if(reader.name() == "row") {
                    qDebug() << reader.attributes().value("C2").toString();
                    reader.skipCurrentElement();
                }
            }
        }
    }
    qDebug() << "end";
}

