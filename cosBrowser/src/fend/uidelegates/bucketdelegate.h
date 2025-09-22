#ifndef BUCKETDELEGATE_H
#define BUCKETDELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>

/*
    这段代码是继承 QStyledItemDelegate 来自定义 代理 (delegate)，目的是在
    QTableView / QTreeView 的某一列（这里是第 1 列，column == 1）显示并
    编辑为一个 下拉框 QComboBox，而不是默认的文本编辑框。

    进入编辑状态时 —— 我要用什么控件（这里是 QComboBox）来展示和修改数据。

    退出编辑状态时 —— 我要怎么把修改后的值写回到 model 里。
*/


class BucketDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    BucketDelegate(QObject *parent = nullptr);

protected:
    // createEditor是为了创建一个combobox编辑器控件
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    // setEditorData是为了把数据从model传递到editor
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    // setModelData是为了把数据从editor传递到model
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // BUCKETDELEGATE_H
