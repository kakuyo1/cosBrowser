#include "pageflipwidget.h"
#include "ui_pageflipwidget.h"

PageFlipWidget::PageFlipWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PageFlipWidget)
{
    ui->setupUi(this);
}

PageFlipWidget::~PageFlipWidget()
{
    delete ui;
}
