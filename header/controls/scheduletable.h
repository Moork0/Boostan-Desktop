#ifndef SCHEDULETABLE_H
#define SCHEDULETABLE_H

/*
    * Class: ScheduleTable
    * Files: scheduletable.h and scheduletable.cpp
    * This is a Back-end class for QML component named ScheduleTable
    * The main job of this class is to:
    * 1- Serialize/deSerialize the model. 2- Find collisions in model
*/

#include <QObject>
#include <QVariant>
#include <QDataStream>
#include <QDebug>

class ScheduleTable : public QObject
{
    Q_OBJECT

private:

    // Container that we store our model in
    QHash<QString, QVariantMap> model_data;

public:

    // Type of collisions
    enum collision_errors
    {
        NoCollision = 0,
        ExamCollision,
        TimeCollision,
        ExamWarning
    };

    explicit        ScheduleTable(QObject *parent = nullptr);

    // calculate the corresponding row(in scheduleTable component) for the given day
    static int      calculateScheduleRow    (const QString& day);
    // calculate the corresponding column(in scheduleTable component) for the given hour
    static float    calculateScheduleColumn (const QString& hour);
    // calculate the the duration of a course in hour format.
    static float    calculateScheduleLen    (const QString& hour, const float start_column);

public slots:
    // add element to model_data
    void            addEelement             (const QString uid, QVariantMap element);
    // remove element with key 'uid' from model_data
    void            removeEelement          (const QString& uid);
    // check for collisions between element and other members of model_data
    // return a list of members with collision and the collision type.
    QVariantList    checkCollision          (const QVariantMap element) const;
    // clear model_data
    void            clearAll();
    // serialize the model_data into a string(Base64 format)
    QString         serialize() const;

    // parse 'data'(Base64 format) and return a container in type of model_data
    static QHash<QString, QVariantMap> deserialize(const QString& data);

    /** specialized function member for storing courses **/

    // Generate a Unique id for a course
    static QString  getUid                  (const int course_number, const int course_group);
    static QString  getUid                  (const QString& course_number, const QString& course_group);

    // return a string which in have list of course names separated with '<br>'
    QString         getCourseNames          (const QVariantList uids) const;
    // set 'warning_list' as a list of warnings for course with uid of 'uid'
    void            setCourseWarnings       (const QString uid, const QVariantList warning_list);
};

#endif // SCHEDULETABLE_H
