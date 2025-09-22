#include "bucketswidget.h"
#include "src/middle/managerglobal.h"
#include "src/middle/managermodels.h"
#include "ui_bucketswidget.h"

BucketsWidget::BucketsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BucketsWidget)
{
    ui->setupUi(this);
    ui->listViewBuckets->setModel(MANAGER_GLOBAL->managerModels->modelBuckets());
}

BucketsWidget::~BucketsWidget()
{
    delete ui;
}
