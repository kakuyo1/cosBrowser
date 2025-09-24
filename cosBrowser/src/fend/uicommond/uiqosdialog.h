#ifndef UIQOSDIALOG_H
#define UIQOSDIALOG_H

#include "basedialog.h"

#include <QDialog>

namespace Ui {
class uiQosDialog;
}

class uiQosDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit uiQosDialog(QWidget *parent = nullptr);
    ~uiQosDialog();

private:
    Ui::uiQosDialog *ui;
};

#endif // UIQOSDIALOG_H
