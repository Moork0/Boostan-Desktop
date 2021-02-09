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

int OfferedCourseModel::getRole(OfferedCourseModel::roles role)
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

void OfferedCourseModel::setDataContainer(QHash<QString, QVariantList*>& container)
{
    cleanUp();
    for (QVariantList* element : container) {
        data_container.append(element);
    }
}

QVariantMap OfferedCourseModel::getCourse(int index) const
{
    QVariantMap map;
    if (index < 0 || index >= data_container.size()) return map;
    map[QStringLiteral("teacher")] = data_container[index]->at(getRole(teacherRole));
    map[QStringLiteral("name")] = data_container[index]->at(getRole(courseNameRole));
    QStringList times =data_container[index]->at(getRole(timeRole)).toString().split("<br>");
    QVariantList rows, columns;
    for (QString& time : times) {
        time = time.trimmed();
        // the length of '00:00-00:00' is 11. so for extract the they we don't need the last 12 char's
        rows.append(calculateScheduleRow(time.chopped(12)));
        columns.append(calculateScheduleColumn(time.right(11)));
    }
    map[QStringLiteral("row")] = rows;
    map[QStringLiteral("column")] = columns;
    map[QStringLiteral("code")] = data_container[index]->at(getRole(groupRole)).toString()
            + data_container[index]->at(getRole(courseNumberRole)).toString();

    return map;
}


int OfferedCourseModel::calculateScheduleRow(const QString &day) const
{
    for (int i {0}; i < 5; ++i) {
        if (day.startsWith(days_keyword[i])) {
            return i;
        }
    }
    return -1;
}

int OfferedCourseModel::calculateScheduleColumn(const QString &hour) const
{
    for (int i {0}; i < 5; ++i) {
        if (hour.startsWith(hours_keyword[i])) {
            return i;
        }
    }
    return -1;
}
