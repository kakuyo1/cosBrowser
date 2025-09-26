#include "comboline.h"

#include <QAbstractItemView>
#include <QCompleter>

#include <src/fend/uidelegates/tableitemdelegate.h>

ComboLine::ComboLine(QWidget* parent) : QLineEdit(parent) {
    ComboLine({}, parent);
}

ComboLine::ComboLine(const QStringList &items, QWidget *parent) : QLineEdit(parent)
{
    setItems(items);
}

ComboLine::~ComboLine()
{

}

void ComboLine::setItems(const QStringList &items)
{
    QCompleter* completer = new QCompleter(items, this);
    // completer->popup()获取到与补全器关联的下拉列表视图
    QAbstractItemView* popupView = completer->popup();
    popupView->setStyleSheet(R"(
    QListView::item:selected {
        background-color: #d0e7ff;  /* 浅蓝背景 */
        color: blue;               /* 文字变蓝 */
    }
    QListView::item:hover {
        background-color: #e6f2ff; /* 鼠标悬停时背景 */
    }
    )");

    // 隐藏下拉表的滚动条
    popupView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    popupView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 设置自定义的item代理 : 目的是重绘某些QSS不支持的属性，如整行高亮，调整行高
    popupView->setItemDelegate(new TableItemDelegate(popupView));
    popupView->setCursor(Qt::PointingHandCursor); // 设置鼠标形状
    // 关联信号与槽
    connect(completer, QOverload<const QString &>::of(&QCompleter::activated),
            this, [=](const QString& itemText){
        emit itemSelected(itemText);
    });
    setCompleter(completer); // 设置补全器

    //设置补全模式
    completer->setFilterMode(Qt::MatchContains); // 包含匹配 : "ab" 可以匹配 "abc", "cab"
}

void ComboLine::setItems(const QList<int> items)
{
    QStringList strs;
    for (int item : items) {
        strs.append(QString::number(item));
    }
    setItems(strs);
}

void ComboLine::setItems(int start, int len)
{
    QStringList strs;
    for (int i = start; i < start + len; ++i) {
        strs.append(QString::number(i));
    }
    setItems(strs);
}

QStringList ComboLine::items() const
{
    QStringList list{};
    if (completer()) { // completer 可能为空
        QAbstractItemModel* model = completer()->model();
        for (int i = 0; i < model->rowCount(); ++i) {
            QModelIndex index = model->index(i, 0);
            list.append(model->data(index).toString());
        }
    }
    return list;
}

void ComboLine::mousePressEvent(QMouseEvent *event)
{
    if (completer()) {
        completer()->complete(); // 显示下拉列表
    }
    QLineEdit::mousePressEvent(event);
}
