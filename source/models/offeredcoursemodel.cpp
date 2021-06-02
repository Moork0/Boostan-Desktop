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
    return _data_container.size();
}

QVariant OfferedCourseModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    int row = index.row();
    int column = role - ROLE_START - 1;
    if (row < 0 || row >= _data_container.size()) return QVariant();

    return _data_container.at(row)->at(column);
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
    if (row < 0 || row >= _data_container.size()) return false;

    _data_container[row]->replace(column , value);
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
    if (_data_container.isEmpty()) return;
    for(QVariantList* element : qAsConst(_data_container)) {
        delete element;
        element = nullptr;
    }
    _data_container.clear();
}

void OfferedCourseModel::setDataContainer(QList<QVariantList*>& container)
{
    cleanUp();
    _data_container.swap(container);
}

QVariantMap OfferedCourseModel::toScheduleFormat(const int index) const
{
    QVariantMap map;
    if (index < 0 || index >= _data_container.size()) return map;
    map[QStringLiteral("teacher")] = _data_container[index]->at(roleToIndex(teacherRole));
    map[QStringLiteral("name")] = _data_container[index]->at(roleToIndex(courseNameRole));
    map[QStringLiteral("exam")] = _data_container[index]->at(roleToIndex(examRole)).toString().replace(QStringLiteral("<br>"), QStringLiteral(" "));
    map[QStringLiteral("warningForCourses")] = QVariantList();
    map[QStringLiteral("uid")] = ScheduleTable::getUid(_data_container[index]->at(roleToIndex(courseNumberRole)).toString(), _data_container[index]->at(roleToIndex(groupRole)).toString());

    QStringList times = _data_container[index]->at(roleToIndex(timeRole)).toString().split("<br>");
    QVariantList rows, columns, lengths;
    float calculated_column;
    for (QString& time : times) {
        time = time.trimmed();
        // the first 12 character of the 'time' is name of the day.
        rows.append(ScheduleTable::calculateScheduleRow(time.chopped(12)));
        // the length of '00:00-00:00' is 11. (the last 11 character of the 'time' string)
        calculated_column = ScheduleTable::calculateScheduleColumn(time.right(11));
        columns.append(calculated_column);
        lengths.append(ScheduleTable::calculateScheduleLen(time.right(11), calculated_column));
    }
    map[QStringLiteral("row")] = rows;
    map[QStringLiteral("column")] = columns;
    map[QStringLiteral("length")] = lengths;

    return map;
}

int OfferedCourseModel::getCourseWeight(const int index) const
{
    return _data_container.at(index)->at(roleToIndex(weightRole)).toInt();
}

void OfferedCourseModel::clearAllChoosed(const QList<int> index_list)
{
    for (uint index : index_list) {
        setData(QAbstractItemModel::createIndex(index, 0), false, isChoosedRole);
    }
}

