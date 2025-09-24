#ifndef BREADCRUMDWIDGET_H
#define BREADCRUMDWIDGET_H

#include <QStandardItemModel>
#include <QWidget>

namespace Ui {
class BreadCrumdWidget;
}

class BreadCrumdWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BreadCrumdWidget(QWidget *parent = nullptr);
    ~BreadCrumdWidget();

    void setPath(const QString& path);
    void setPath(const QStringList& pathList);
    void clearPath();

    QString currentPath() const;
    QString rootName() const;

    bool isEmptyPath() const;
    bool isRootPath() const;

signals:
    void pathChanged(const QString& newpath);
    void refresh(const QString& path);

private:
    void addPathItem(const QString& name); // 添加路径项（即按钮）
    void onItemClicked(const QModelIndex& index); // 路径项点击事件
    QStandardItem* currentPathItem () const; // 获取当前路径项
    QString getFullPathByItem(QStandardItem* item = nullptr) const; // 根据路径项获取完整路径
private:
    Ui::BreadCrumdWidget *ui;
    /*
     根目录必须存在，m_rootName 用于保存根目录名称
     例如：根目录为 "/" , m_rootName = "/"
     model存储所有目录名称列表
    */
    QString m_rootName;
    QStandardItemModel* m_model;
};

#endif // BREADCRUMDWIDGET_H
