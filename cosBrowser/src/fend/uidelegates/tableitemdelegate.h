#ifndef TABLEITEMDELEGATE_H
#define TABLEITEMDELEGATE_H

#include <QStyledItemDelegate>

class TableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TableItemDelegate(QAbstractItemView* table, QObject *parent = nullptr);
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    QAbstractItemView* m_table; // 被代理的表格视图view，方便获取整行的item
};

#endif // TABLEITEMDELEGATE_H
