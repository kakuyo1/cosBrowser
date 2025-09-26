#include "uitransfer.h"
#include "ui_uitransfer.h"

UiTransfer::UiTransfer(QWidget *parent)
    : uiQosWidget(parent)
    , ui(new Ui::UiTransfer)
{
    ui->setupUi(body());
    setTitleText("传输列表");
    setFixedSize(600, 450);

    ui->tabWidget->setStyleSheet(R"(QTabWidget::pane {
    border: 1px solid #cccccc;
    background: #f9f9f9;
}
QTabBar::tab {
    background: #eeeeee;
    border: 1px solid #cccccc;
    padding: 6px 12px;
}
QTabBar::tab:selected {
    background: #ffffff;
    border-bottom: 2px solid #0078d7; /* 选中高亮线 */
}
QTabBar::tab:!selected {
    margin-top: 2px; /* 未选中的标签下移，突出层次 */
}
)");

}

UiTransfer::~UiTransfer()
{
    delete ui;
}
