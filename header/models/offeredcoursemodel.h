#ifndef OFFEREDCOURSEMODEL_H
#define OFFEREDCOURSEMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <vector>
#include <QDebug>

using ContainerType = std::vector<std::vector<QVariant>*>;

class OfferedCourseModel : public QAbstractListModel
{
    Q_OBJECT

private:
    // the container which stores the data
    ContainerType* data_container;

    enum
    {
        ROLE_START = Qt::UserRole + 1,
        groupRole,
        courseNumberRole,
        courseNameRole,
        weightRole,
        capacityRole,
        sexRole,
        teacherRole,
        timeRole,
        placeRole,
        examRole,
        selectedRole,
        ROLE_END
    };

public:
    explicit OfferedCourseModel(QObject *parent = nullptr);
    virtual ~OfferedCourseModel();

    inline static const QStringList columns
    {
        QStringLiteral("group"), QStringLiteral("courseNumber"), QStringLiteral("courseName"),
        QStringLiteral("weight"), QStringLiteral("capacity"), QStringLiteral("sex"),
        QStringLiteral("teacher"), QStringLiteral("time"), QStringLiteral("place"),
        QStringLiteral("exam"), QStringLiteral("selected")
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // custom functions
    void cleanUp();
    void setDataContainer (ContainerType* container);

};

#endif // OFFEREDCOURSEMODEL_H
