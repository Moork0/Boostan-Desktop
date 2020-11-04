#include "header/courseschedulehandler.h"

CourseScheduleHandler::CourseScheduleHandler()
{
    QList<QVariant> temp;
    weekly_schedule.reserve(5);
    temp.reserve(5);
    for (int i{0}; i < 5; ++i) {
        temp.append(QVariantMap {{QString("name"), ""}, {QString("exam"), ""}, {QString("teacher"), ""}});
    }
    for (int i{0}; i < 5; ++i) {
        weekly_schedule.append(temp);
    }
}

void CourseScheduleHandler::extractWeeklySchedule(QString& response)
{
//    QRegularExpression re {xmldata_pattern, QRegularExpression::UseUnicodePropertiesOption};
//    QRegularExpressionMatch match {re.match(response)};

//    if (!match.hasMatch()) return;

//    QXmlStreamReader reader(match.captured());
    QXmlStreamReader reader {response};
    QMap<QString, QVariant> course_data;
    QString hour;

    if (!reader.readNextStartElement()) return;
    if (reader.name() != "Root") return;

    while(reader.readNextStartElement()) {
        if(reader.name() != "row") continue;

        QXmlStreamAttributes attribute {reader.attributes()};
        course_data["name"] = attribute.value("C2").toString();
        course_data["teacher"] = attribute.value("C4").toString();
        course_data["exam"] = attribute.value("C13").toString();

        // 5 is the number of a week days
        for (int day_index{0}; day_index < 5; ++day_index) {
            hour = attribute.value("C" + QString::number(day_index + 5)).toString();
            if (hour == "") continue;
            weekly_schedule[day_index][hourIndex(hour)] = course_data;
        }
        reader.skipCurrentElement();
    }

//    qDebug() << weekly_schedule;
}

QList<QVariant> CourseScheduleHandler::DailyScheduleModel(int day) const
{
    return weekly_schedule[day];
}

int CourseScheduleHandler::hourIndex(QString& hour) const
{
    const QStringList hours{"08", "10", "13", "15", "17"};
    for (int i{0}; i < hour.size(); ++i) {
        if (hour.startsWith(hours[i])) {
            return i;
        }
    }
    return -1;
}


