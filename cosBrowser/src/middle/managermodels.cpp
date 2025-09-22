#include "managermodels.h"
#include "src/middle/managerglobal.h"

#include <src/middle/models/mybucket.h>

#include <src/middle/signals/managersignals.h>

ManagerModels::ManagerModels(QObject *parent)
    : QObject{parent},
      m_modelBuckets(new QStandardItemModel(this))
{
    // 连接信号槽，当获取到存储桶列表时更新模型
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::bucketsSuccess,
            this, &ManagerModels::setBuckets);
}

QStandardItemModel *ManagerModels::modelBuckets() const
{
    return m_modelBuckets;
}

void ManagerModels::setBuckets(const QList<MyBucket> &buckets)
{
    m_modelBuckets->setRowCount(buckets.size()); // 行
    for (int i = 0; i < buckets.size(); ++i) {
        const MyBucket& bucket = buckets.at(i);
        // 第一列
        QModelIndex index_name = m_modelBuckets->index(i, 0);
        m_modelBuckets->setData(index_name, bucket.name);
        m_modelBuckets->setData(index_name, QString("存储桶名称: %1").arg(bucket.name), Qt::ToolTipRole);
        // 第二列
        QModelIndex index_location = m_modelBuckets->index(i,1);
        m_modelBuckets->setData(index_location, bucket.location);
        // 第三列
        QModelIndex index_create_date = m_modelBuckets->index(i,2);
        m_modelBuckets->setData(index_create_date, bucket.createDate);
    }
    // 默认按照时间排序，降序，以便最新创建的存储桶显示在最上面
    m_modelBuckets->sort(2, Qt::DescendingOrder);
}
