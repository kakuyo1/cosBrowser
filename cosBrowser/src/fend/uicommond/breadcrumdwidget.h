#ifndef BREADCRUMDWIDGET_H
#define BREADCRUMDWIDGET_H

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

private:
    Ui::BreadCrumdWidget *ui;
};

#endif // BREADCRUMDWIDGET_H
