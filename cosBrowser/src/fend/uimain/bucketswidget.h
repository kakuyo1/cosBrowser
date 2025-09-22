#ifndef BUCKETSWIDGET_H
#define BUCKETSWIDGET_H

#include <QWidget>

namespace Ui {
class BucketsWidget;
}

class BucketsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BucketsWidget(QWidget *parent = nullptr);
    ~BucketsWidget();

private:
    Ui::BucketsWidget *ui;
};

#endif // BUCKETSWIDGET_H
