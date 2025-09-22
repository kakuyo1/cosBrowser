#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include <QWidget>

namespace Ui {
class ToolBarWidget;
}

class ToolBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBarWidget(QWidget *parent = nullptr);
    ~ToolBarWidget();

signals:
    void quitLogin();

private slots:
    void on_btnUpLoad_clicked();

    void on_btnDownLoad_clicked();

private:
    Ui::ToolBarWidget *ui;
};

#endif // TOOLBARWIDGET_H
