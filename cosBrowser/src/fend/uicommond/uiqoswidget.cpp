#include "uiqoswidget.h"
#include "ui_uiqoswidget.h"
#include "src/config/global.h"

uiQosWidget::uiQosWidget(QWidget *parent)
    : BaseDialog(parent)
    , ui(new Ui::uiQosWidget)
{
    ui->setupUi(body()); // 注意这里传入的是 body()
    setLogoImage(CONFIG::PATH::LOGO_PATH);
    setTitleText("Qos");
    setLogoFixedSize(40, 40);
    setTitleProperty("TitleStyle", "h4");
    // this->setWindowIcon(QIcon(":/static/image/icontt.ico"));
    // resize(400,50);
}

uiQosWidget::~uiQosWidget()
{
    delete ui;
}
