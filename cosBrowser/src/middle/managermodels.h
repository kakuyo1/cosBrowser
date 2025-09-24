#ifndef MANAGERMODELS_H
#define MANAGERMODELS_H

#include <QObject>
#include <QStandardItemModel>
#include <QList>
#include "src/middle/models/mybucket.h"
class ManagerModels : public QObject
{
    Q_OBJECT
public:
    explicit ManagerModels(QObject *parent = nullptr);
    QStandardItemModel *modelBuckets() const;
    QStandardItemModel *modelObjects() const;

signals:

private slots:
    void setBuckets(const QList<MyBucket>& buckets);
    void setObjects(const QList<MyObject>& objects);

private:
    void initBucketsTable();
    void initObjectsTable();
private:
    QStandardItemModel* m_modelBuckets = nullptr;
    QStandardItemModel* m_modelObjects = nullptr;
};

#endif // MANAGERMODELS_H
