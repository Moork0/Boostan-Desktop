#include "header/models/offeredcoursemodel.h"

OfferedCourseModel::OfferedCourseModel(QObject *parent)
    : QAbstractListModel(parent), data_container {nullptr}
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
    if (data_container == nullptr) return 0;
    return data_container->size();
}

QVariant OfferedCourseModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || data_container == nullptr) return QVariant();

    size_t row = index.row();
    int column = role - ROLE_START - 1;
    if (row >= data_container->size()) return QVariant();

    return data_container->at(row)->at(column);
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
    if (data(index, role) == value || data_container == nullptr) return false;

    size_t row = index.row();
    int column = role - ROLE_START - 1;
    if (row >= data_container->size()) return false;

    data_container->at(row)->at(column) = value;
    qDebug() << data_container->at(row)->at(column);
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
    if (!data_container) return;
    for (std::vector<QVariant>* element : *data_container) {
        delete element;
    }
    delete data_container;
    data_container = nullptr;
}

void OfferedCourseModel::setDataContainer(ContainerType* container)
{
    cleanUp();
    data_container = container;
}
