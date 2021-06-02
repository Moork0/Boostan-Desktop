#ifndef OFFEREDCOURSEMODEL_H
#define OFFEREDCOURSEMODEL_H

/*
    * Class: OfferedCourseModel
    * Files: offeredcoursemodel.h, offeredcoursemodel.cpp
    * This class have to prepare data model to be used in a table in OfferedCourse page
    * Also this class convert a data to ScheduleTable-compatible format to get used there.
*/

#include <QAbstractListModel>
#include <QHash>
#include <QDebug>

#include "../controls/scheduletable.h"

class OfferedCourseModel : public QAbstractListModel
{
    Q_OBJECT

private:
    // the container which stores the data
    QList<QVariantList*>    _data_container;

public:
    explicit    OfferedCourseModel          (QObject *parent = nullptr);
    virtual     ~OfferedCourseModel         ();

    // role names used in model
    inline static const QStringList columns
    {
        QStringLiteral("group"), QStringLiteral("courseNumber"), QStringLiteral("courseName"),
        QStringLiteral("weight"), QStringLiteral("capacity"), QStringLiteral("sex"),
        QStringLiteral("teacher"), QStringLiteral("time"), QStringLiteral("place"),
        QStringLiteral("exam"), QStringLiteral("isChoosed")
    };

    // roles used in model
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

    Q_ENUMS(gender)

    /** Basic functionality: **/

    int             rowCount            (const QModelIndex &parent = QModelIndex()) const override;
    QVariant        data                (const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool            setData             (const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags   flags               (const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames    () const override;

    /** Custom functions: **/

    // convert {{role}} to integer index
    static int      roleToIndex         (roles role);
    // free up memory
    void            cleanUp             ();
    // move {{container}} data's to {{_data_container}}
    void            setDataContainer    (QList<QVariantList*>& container);

public slots:
    // convert and return a 'data_container' element to ScheduleTable compatible format
    QVariantMap     toScheduleFormat    (const int index) const;
    // return a weight of a course at index {{index}} in {{_data_container}}
    int             getCourseWeight     (const int index) const;
    // disable isChoosed property of {{index_list}} indexes
    void            clearAllChoosed          (const QList<int> index_list);
};

#endif // OFFEREDCOURSEMODEL_H
