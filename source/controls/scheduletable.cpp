#include "header/controls/scheduletable.h"
#include "header/helpers/commonmacros.h"

/*
    Our data structure is (at least)something like this:
    {
        element_UniqueId: {"name": "Course1", "row": [1, 2], "column": [2, 2], "length": [2, 3]}
    }

    It could have many other members but the members above are necessary for functionality.
    So, imagine a schedule with week days(starting at Saturday) as rows(starting at 0)
    and day hours(starting at 08:00) as columns(starting at 0).

    Thus the structure above would translated to something like this:
    "Course1" will held in sunday and monday both at 10:00
    and the sunday class would take long 2 blocks of time(each block represent an hour in here).
    So, "Course1" would be held in Sunday, at 10:00 and would take long 2 hours.
*/
ScheduleTable::ScheduleTable(QObject *parent) : QObject(parent)
{

}

int ScheduleTable::calculateScheduleRow(const QString& day)
{
    // list of days
    static const QStringList days_keyword{ MyStringLiteral("شنبه"), MyStringLiteral("يک"), MyStringLiteral("دو"), MyStringLiteral("سه"), MyStringLiteral("چهار"), MyStringLiteral("پنج"), MyStringLiteral("جمعه") };
    static const int keyword_size {days_keyword.size()};

    for (int i {0}; i < keyword_size; ++i) {
        if (day.startsWith(days_keyword[i])) {
            return i;
        }
    }
    return -1;
}

float ScheduleTable::calculateScheduleColumn(const QString& hour)
{
    // 8:00 is hour that classes could start(We have no class befor 08:00)
    static constexpr int first_hour {8};
    // 20:00 is hour that classes could end(We have no class after 20:00)
    static constexpr int last_hour {20};
    static constexpr int columns_length {last_hour - first_hour};

    QString current_hour;
    // iterate over hours between first_hour and last_hour
    // and find the correct hour and corresponding column number (i)
    /// TODO: There must be more efficient way instead of iteration. Find that!
    for (int i {0}; i <= columns_length; ++i) {
        current_hour = QString::number(first_hour + i);
        if (current_hour.size() == 1)
            current_hour = QString(QStringLiteral("0")) + current_hour;

        // Divide minutes by 60 to have result as hour. for examle: 10:30 => column number 2.5
        if (hour.startsWith(current_hour))
            return i + (hour.midRef(3, 2).toFloat() / 60);
    }
    return -1;
}

float ScheduleTable::calculateScheduleLen(const QString& hour, const float start_column)
{
    // 5 is the length of the last 5 character of "12:34-56:78" which is "56:78"
    float end_column {calculateScheduleColumn(hour.right(5))};
    return end_column - start_column;
}

void ScheduleTable::addEelement(const QString uid, QVariantMap element)
{
    model_data.insert(uid, element);
}

void ScheduleTable::removeEelement(const QString &uid)
{
    model_data.remove(uid);
}

QVariantList ScheduleTable::checkCollision(const QVariantMap element) const
{
    // initialize the necessary variables from 'element'
    const QVariantList columns (element.value("column").toList());
    const QVariantList rows (element.value("row").toList());
    const QVariantList lengths (element.value("length").toList());
    const QStringList exam {element.value("exam").toString().split(QStringLiteral("<br>"))};
    // number of course sessions in a week
    const int sessions_number {columns.size()};

    // Initialize the necessary variables for being used in loop
    QHash<QString, QVariantMap>::const_iterator iterator = model_data.cbegin();
    QHash<QString, QVariantMap>::const_iterator end = model_data.cend();
    QVariantList iterator_columns, iterator_rows, iterator_lengths, exam_warnings;
    QVariantMap iterator_value;
    QStringList iterator_exam;

    // iterate over model_data elements
    for (; iterator != end; ++iterator) {

        iterator_value = iterator.value();
        iterator_exam = iterator_value.value("exam").toString().split(QStringLiteral("<br>"));
        // check for any collision for exam times
        for (int itexam_index {0}; itexam_index < iterator_exam.size(); ++itexam_index) {
            if (iterator_exam.at(itexam_index) == MyStringLiteral("نامشخص"))
                continue;

            for (int exam_index {0}; exam_index < exam.size(); ++exam_index) {
                if (exam.at(exam_index) == MyStringLiteral("نامشخص"))
                    continue;

                if (iterator_exam.at(itexam_index) == exam.at(exam_index))
                    return QVariantList {ExamCollision, iterator_value.value("name").toString()};

                /// TODO: should find better way because there is no unique format of exam. unless I normalize them
                // we assume exam format is smt like this: 12.01/08:00. so the first 5 chars are representing the date.
                // Exams have no collision but they are in a same day. Add to warnings
                if (iterator_exam.at(itexam_index).leftRef(5) == exam.at(exam_index).leftRef(5))
                    exam_warnings.append(iterator.key());
            }
        }

        iterator_columns = iterator_value.value("column").toList();
        iterator_rows = iterator_value.value("row").toList();
        iterator_lengths = iterator_value.value("length").toList();

        // Check for collision in course times
        int size {iterator_columns.size()};
        for (int iter_index {0}; iter_index < size; ++iter_index) {
            float iter_column {iterator_columns[iter_index].toFloat()};
            float iter_len {iterator_lengths[iter_index].toFloat()};

            // iterate over 'element' rows and columns
            for (int orig_index {0}; orig_index < sessions_number; ++orig_index) {
                if (iterator_rows[iter_index] != rows[orig_index]) continue;
                float orig_column {columns[orig_index].toFloat()};
                float orig_len {lengths[orig_index].toFloat()};

                if (iter_column >= orig_column && iter_column < (orig_column + orig_len))
                    return QVariantList {TimeCollision, iterator_value.value("name").toString()};

                if (orig_column >= iter_column && orig_column < (iter_column + iter_len))
                    return QVariantList {TimeCollision, iterator_value.value("name").toString()};
            } // end of third 'for'

        } // end of second 'for'

    } // end of first 'for'

    if (!exam_warnings.isEmpty())
        return QVariantList {ExamWarning, exam_warnings};

    return QVariantList {NoCollision, QString()};

}

void ScheduleTable::clearAll()
{
    model_data.clear();
}

QString ScheduleTable::serialize() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << model_data;
    return data.toBase64();
}

QHash<QString, QVariantMap> ScheduleTable::deserialize(const QString& data)
{
    QHash<QString, QVariantMap> container;
    QByteArray raw_data {data.toUtf8()};
    // convert to binary
    raw_data = QByteArray::fromBase64(raw_data);
    QDataStream stream(raw_data);
    stream >> container;
    return container;
}

QString ScheduleTable::getUid(const int course_number, const int course_group)
{
    return QString::number(course_number) + QString::number(course_group);
}

QString ScheduleTable::getUid(const QString &course_number, const QString &course_group)
{
    return course_number + course_group;
}

QString ScheduleTable::getCourseNames(const QVariantList uids) const
{
    QString names;
    for (const QVariant& uid : uids) {
        names.append( model_data.value(uid.toString()).value("name").toString() + QStringLiteral("<br>"));
    }
    return names;
}

void ScheduleTable::setCourseWarnings(const QString uid, const QVariantList warning_list)
{
    if (!model_data.contains(uid)) return;
    model_data[uid][QStringLiteral("warningForCourses")] = warning_list;
}

