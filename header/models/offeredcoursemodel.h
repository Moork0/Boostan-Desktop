#ifndef OFFEREDCOURSEMODEL_H
#define OFFEREDCOURSEMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <vector>
#include <QDebug>


class OfferedCourseModel : public QAbstractListModel
{
    Q_OBJECT

private:
    // the container which stores the data
    QList<QVariantList*>    data_container;
    // list of selected courses indexes(row numbers)
    QMap<int, QVariantMap>              choosed_list;

    int         calculateScheduleRow(const QString& day) const;
    float       calculateScheduleColumn(const QString& hour) const;
    float       calculateScheduleLen(const QString& hour, const float start_column) const;

public:
    explicit OfferedCourseModel(QObject *parent = nullptr);
    virtual ~OfferedCourseModel();

    inline static const QStringList columns
    {
        QStringLiteral("group"), QStringLiteral("courseNumber"), QStringLiteral("courseName"),
        QStringLiteral("weight"), QStringLiteral("capacity"), QStringLiteral("sex"),
        QStringLiteral("teacher"), QStringLiteral("time"), QStringLiteral("place"),
        QStringLiteral("exam"), QStringLiteral("isChoosed")
    };

    enum roles
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
        isChoosedRole,
        ROLE_END
    };

    enum gender
    {
        Male = 0,
        Female,
        None
    };

    enum collision_errors
    {
        NoCollision = 0,
        ExamCollision,
        TimeCollision,
    };

    Q_ENUMS(gender)
    Q_ENUMS(collision_errors)

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // custom functions
    static int roleToIndex(roles role);
    void cleanUp();
    void setDataContainer (const QHash<QString, QVariantList*>& container);

public slots:
    QVariantMap toScheduleFormat(int index) const;
    void appendChoosedList(const int row_number);
    void removeChoosedList(const int row_number);
};

#endif // OFFEREDCOURSEMODEL_H
