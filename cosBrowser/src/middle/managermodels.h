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

signals:

private slots:
    void setBuckets(const QList<MyBucket>& buckets);

private:
    QStandardItemModel* m_modelBuckets = nullptr;
};

#endif // MANAGERMODELS_H
