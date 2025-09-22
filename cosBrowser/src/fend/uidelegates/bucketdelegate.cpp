#include "bucketdelegate.h"

#include <QComboBox>

BucketDelegate::BucketDelegate(QObject *parent) : QStyledItemDelegate(parent) {}


// 当 编辑第 1 列 时，使用一个 QComboBox 下拉框，里面预置了三个选项。
QWidget *BucketDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
        Q_UNUSED(option);
        Q_UNUSED(index);

        QComboBox* box = new QComboBox(parent);
        box->addItem("ap-beijing");
        box->addItem("ap-shanghai");
        box->addItem("ap-guangzhou");
        box->setFrame(false); // 去掉边框
        return box;
}
// 当进入编辑模式时，把 model 里的数据 同步到编辑控件中。
void BucketDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
        QComboBox* box = static_cast<QComboBox*>(editor);
        QString text = index.model()->data(index, Qt::EditRole).toString();
        box->setCurrentText(text);
}
// 当用户编辑完并确认（比如回车或失去焦点）时，把 编辑器里的数据 回写到 model 中。
void BucketDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
        QComboBox* box = static_cast<QComboBox*>(editor);
        model->setData(index, box->currentText(), Qt::EditRole);
}
