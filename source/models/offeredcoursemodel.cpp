#include "hedaer/models/offeredcoursemodel.h"

OfferedCourseModel::OfferedCourseModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QVariant OfferedCourseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

int OfferedCourseModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant OfferedCourseModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}

QHash<int, QByteArray> OfferedCourseModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    int column_number{0};
    for (int i{ROLE_START + 1}; i != ROLE_END; ++i, ++column_number) {
        roles.insert(i, this->columns[column_number]);
    }
    return roles;
}
