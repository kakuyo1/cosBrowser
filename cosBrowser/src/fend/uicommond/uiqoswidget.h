#ifndef UIQOSWIDGET_H
#define UIQOSWIDGET_H

#include "basedialog.h"

#include <QWidget>

namespace Ui {
class uiQosWidget;
}

class uiQosWidget : public BaseDialog
{
    Q_OBJECT

public:
    explicit uiQosWidget(QWidget *parent = nullptr);
    ~uiQosWidget();

private:
    Ui::uiQosWidget *ui;
};

#endif // UIQOSWIDGET_H
