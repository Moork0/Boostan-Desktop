#include "header/controls/scheduletable.h"

ScheduleTable::ScheduleTable(QObject *parent) : QObject(parent)
{

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
    const QVariantList columns (element.value("column").toList());
    const QVariantList rows (element.value("row").toList());
    const QVariantList lengths (element.value("length").toList());
    const QStringList exam {element.value("exam").toString().split(QStringLiteral("<br>"))};
    const int sessions_number {columns.size()};

    QHash<QString, QVariantMap>::const_iterator iterator = model_data.cbegin();
    QHash<QString, QVariantMap>::const_iterator end = model_data.cend();
    QVariantList iterator_columns, iterator_rows, iterator_lengths, exam_warnings;
    QVariantMap iterator_value;
    QStringList iterator_exam;
    for (; iterator != end; ++iterator) {

        iterator_value = iterator.value();
        iterator_exam = iterator_value.value("exam").toString().split(QStringLiteral("<br>"));
        for (int itexam_index {0}; itexam_index < iterator_exam.size(); ++itexam_index) {
            for (int exam_index {0}; exam_index < exam.size(); ++exam_index) {

                if (iterator_exam.at(itexam_index) == exam.at(exam_index))
                    return QVariantList {ExamCollision, iterator_value.value("name").toString()};

                // we assume exam format is smt like this: 12.01/08:00. so the first 5 char is the date.
                /// TODO: should find better way because there is no unique format of exam. unless I normalize them
                if (iterator_exam.at(itexam_index).leftRef(5) == exam.at(exam_index).leftRef(5))
                    exam_warnings.append(iterator.key());
            }
        }

        iterator_columns = iterator_value.value("column").toList();
        iterator_rows = iterator_value.value("row").toList();
        iterator_lengths = iterator_value.value("length").toList();

        int size {iterator_columns.size()};
        for (int iter_index {0}; iter_index < size; ++iter_index) {
            float iter_column {iterator_columns[iter_index].toFloat()};
            float iter_len {iterator_lengths[iter_index].toFloat()};

            for (int orig_index {0}; orig_index < sessions_number; ++orig_index) {
                if (iterator_rows[iter_index] != rows[orig_index]) continue;
                float orig_column {columns[orig_index].toFloat()};
                float orig_len {lengths[orig_index].toFloat()};

                if (iter_column >= orig_column and iter_column < (orig_column + orig_len))
                    return QVariantList {TimeCollision, iterator_value.value("name").toString()};

                if (orig_column >= iter_column and orig_column < (iter_column + iter_len))
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

