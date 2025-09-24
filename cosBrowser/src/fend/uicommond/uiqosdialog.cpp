#include "uiqosdialog.h"
#include "ui_uiqosdialog.h"

uiQosDialog::uiQosDialog(QWidget *parent)
    : BaseDialog(parent)
    , ui(new Ui::uiQosDialog)
{
    ui->setupUi(body());
}

uiQosDialog::~uiQosDialog()
{
    delete ui;
}
