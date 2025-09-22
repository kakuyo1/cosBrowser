#include "uimain.h"
#include "src/middle/managerglobal.h"
#include "ui_uimain.h"
#include "../uilogin/logindialog.h"

#include <src/middle/signals/managersignals.h>


UiMain::UiMain(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UiMain)
    , m_loginDialog(nullptr)
{
    ui->setupUi(this);
    // 设置 splitter 的拉伸因子(左右比例为1：4)
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,4);

    //  关联信号与槽
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::loginSuccess, this, &UiMain::show);
}

UiMain::~UiMain()
{
    delete ui;
    if (m_loginDialog) {
        delete m_loginDialog;
        m_loginDialog = nullptr;
    }
}

void UiMain::showLoginDialog()
{
    if (m_loginDialog == nullptr) {
        m_loginDialog = new LoginDialog();
        // 更新登录信息
        m_loginDialog->updateLoginInfo();
        // 登录切换到主窗口
        connect(m_loginDialog, &LoginDialog::accepted, this, &UiMain::show);
        // 主窗口切换到登录(綁定工具欄退出登录按钮)
        connect(ui->widgetToolBar, &ToolBarWidget::quitLogin, this, &UiMain::showLoginDialog);
    }
    // 隐藏主窗口
    hide();
    m_loginDialog->show();
}
