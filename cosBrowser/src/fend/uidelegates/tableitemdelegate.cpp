#include "tableitemdelegate.h"
#include "qabstractitemview.h"

TableItemDelegate::TableItemDelegate(QAbstractItemView* table, QObject *parent)
    : QStyledItemDelegate{parent}, m_table(table)
{}

void TableItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 鼠标悬停时，整行高亮
    if (option.state.testFlag(QStyle::State_MouseOver) && index.isValid()) {
        QAbstractItemModel* model = m_table->model(); // 获取模型
        int row = index.row(); // 获取当前行号
        int columnCount = model->columnCount(); // 获取列数
        for (int i = 0; i < columnCount; ++i) {
            QModelIndex idx = model->index(row, i);
            QStyleOptionViewItem opt = option; // 复制 option
            opt.index = idx; // 设置为当前列的索引
            opt.rect = m_table->visualRect(idx); // 获取当前列的矩形区域
            QStyledItemDelegate::paint(painter, opt, idx); // 绘制当前列
        }
        return;
    }
    QStyledItemDelegate::paint(painter, option, index);
}
