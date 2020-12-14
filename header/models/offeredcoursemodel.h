#ifndef OFFEREDCOURSEMODEL_H
#define OFFEREDCOURSEMODEL_H

#include <QAbstractListModel>

class OfferedCourseModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit OfferedCourseModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // OFFEREDCOURSEMODEL_H
