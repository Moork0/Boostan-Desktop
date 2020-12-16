#ifndef OFFEREDCOURSEMODEL_H
#define OFFEREDCOURSEMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <vector>

using ContainerType = std::vector<std::vector<QVariant>*>;

class OfferedCourseModel : public QAbstractListModel
{
    Q_OBJECT

private:
    // the container which stores the data's
    const ContainerType* data_container;

    const QStringList columns {""}; // FIXME
    // FIXME
    enum {
        ROLE_START = Qt::UserRole + 1,
        groupRole,
        courseNoRole,
        courseNameRole,
        weightRole,
        capacityRole,
        sexRole,
        teacherRole,
        ROLE_END
    };

public:
    explicit OfferedCourseModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    // custom functions
    void setDataContainer (ContainerType* container);

};

#endif // OFFEREDCOURSEMODEL_H
