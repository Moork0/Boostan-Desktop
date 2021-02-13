#include "header/models/offeredcoursemodel.h"

OfferedCourseModel::OfferedCourseModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

OfferedCourseModel::~OfferedCourseModel()
{
    cleanUp();
}

int OfferedCourseModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;
    return data_container.size();
}

QVariant OfferedCourseModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    int row = index.row();
    int column = role - ROLE_START - 1;
    if (row < 0 || row >= data_container.size()) return QVariant();

    return data_container.at(row)->at(column);
}

QHash<int, QByteArray> OfferedCourseModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    int column_number{0};
    for (int i{ROLE_START + 1}; i != ROLE_END; ++i, ++column_number) {
        roles.insert(i, this->columns[column_number].toUtf8());
    }
    return roles;
}

bool OfferedCourseModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) == value) return false;

    int row = index.row();
    int column = role - ROLE_START - 1;
    if (row < 0 || row >= data_container.size()) return false;

    data_container[row]->replace(column , value);
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

Qt::ItemFlags OfferedCourseModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

int OfferedCourseModel::roleToIndex(OfferedCourseModel::roles role)
{
    return role - ROLE_START - 1;
}

void OfferedCourseModel::cleanUp()
{
    if (data_container.isEmpty()) return;
    for(QVariantList* element : qAsConst(data_container)) {
        delete element;
        element = nullptr;
    }
    data_container.clear();
}

void OfferedCourseModel::setDataContainer(const QHash<QString, QVariantList*>& container)
{
    cleanUp();
    for (QVariantList* element : container) {
        data_container.append(element);
    }
}

QVariantMap OfferedCourseModel::toScheduleFormat(const int index) const
{
    QVariantMap map;
    if (index < 0 || index >= data_container.size()) return map;
    map[QStringLiteral("teacher")] = data_container[index]->at(roleToIndex(teacherRole));
    map[QStringLiteral("name")] = data_container[index]->at(roleToIndex(courseNameRole));
    map[QStringLiteral("exam")] = data_container[index]->at(roleToIndex(examRole));

    QStringList times = data_container[index]->at(roleToIndex(timeRole)).toString().split("<br>");
    QVariantList rows, columns, lengths;
    float calculated_column;
    for (QString& time : times) {
        time = time.trimmed();
        // the first 12 character of the 'time' is name of the day.
        rows.append(calculateScheduleRow(time.chopped(12)));
        // the length of '00:00-00:00' is 11. (the last 11 character of the 'time' string)
        calculated_column = calculateScheduleColumn(time.right(11));
        columns.append(calculated_column);
        lengths.append(calculateScheduleLen(time.right(11), calculated_column));
    }
    map[QStringLiteral("row")] = rows;
    map[QStringLiteral("column")] = columns;
    map[QStringLiteral("length")] = lengths;

    return map;
}

void OfferedCourseModel::appendChoosedList(const int row_number)
{
    choosed_list.insert(row_number, toScheduleFormat(row_number));
}

void OfferedCourseModel::removeChoosedList(const int row_number)
{
    choosed_list.remove(row_number);
}

QVariantList OfferedCourseModel::checkCollision(const int row_number)
{
    const QVariantMap course {toScheduleFormat(row_number)};
    const QVariantList columns (course.value("column").toList());
    const QVariantList rows (course.value("row").toList());
    const QVariantList lengths (course.value("length").toList());
    const int sessions_number {columns.size()};

    QMap<int, QVariantMap>::const_iterator iterator = choosed_list.cbegin();
    QMap<int, QVariantMap>::const_iterator end = choosed_list.cend();
    QVariantList iterator_columns, iterator_rows, iterator_lengths;
    QVariantMap iterator_value;
    for (; iterator != end; ++iterator) {
        iterator_value = iterator.value();
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
                    return QVariantList {TimeCollision, course.value("name").toString()};

                if (orig_column >= iter_column and orig_column < (iter_column + iter_len))
                    return QVariantList {TimeCollision, course.value("name").toString()};
            }
        }
    }
    return QVariantList {NoCollision, QString()};
}

int OfferedCourseModel::calculateScheduleRow(const QString& day) const
{
    static const QStringList days_keyword{ QStringLiteral("شنبه"), QStringLiteral("يک"), QStringLiteral("دو"), QStringLiteral("سه"), QStringLiteral("چهار"), QStringLiteral("پنج"), QStringLiteral("جمعه") };
    static const int keyword_size {days_keyword.size()};
    for (int i {0}; i < keyword_size; ++i) {
        if (day.startsWith(days_keyword[i])) {
            return i;
        }
    }
    return -1;
}

float OfferedCourseModel::calculateScheduleColumn(const QString& hour) const
{
    constexpr int first_hour {8};
    constexpr int last_hour {20};
    constexpr int columns_length {last_hour - first_hour};
    QString current_hour;
    for (int i {0}; i < columns_length; ++i) {
        current_hour = QString::number(first_hour + i);
        if (current_hour.size() == 1)
            current_hour = QString(QStringLiteral("0")) + current_hour;

        if (hour.startsWith(current_hour))
            return i + (hour.midRef(3, 2).toFloat() / 60);
    }
    return -1;
}

float OfferedCourseModel::calculateScheduleLen(const QString& hour, const float start_column) const
{
    // 5 is the length of the last 5 character of "12:34-56:78" which is "56:78"
    float end_column {calculateScheduleColumn(hour.right(5))};
    return end_column - start_column;
}




