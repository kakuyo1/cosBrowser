#include "breadcrumdwidget.h"
#include "ui_breadcrumdwidget.h"

BreadCrumdWidget::BreadCrumdWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BreadCrumdWidget)
{
    ui->setupUi(this);
}

BreadCrumdWidget::~BreadCrumdWidget()
{
    delete ui;
}
