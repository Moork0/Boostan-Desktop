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
    qDebug() << data_container.at(row)->at(column);
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

Qt::ItemFlags OfferedCourseModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

void OfferedCourseModel::cleanUp()
{
    if (data_container.isEmpty()) return;
    for (QVariantList* element : data_container) {
        delete element;
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
