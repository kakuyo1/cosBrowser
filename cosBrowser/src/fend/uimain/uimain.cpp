#include "uimain.h"
#include "src/middle/managerglobal.h"
#include "ui_uimain.h"
#include "../uilogin/logindialog.h"
#include "src/config/global.h"
#include <src/middle/signals/managersignals.h>
#include <QPushButton>


UiMain::UiMain(QWidget *parent)
    : uiQosWidget(parent)
    , ui(new Ui::UiMain)
    , m_loginDialog(nullptr)
{
    ui->setupUi(body());
    // 设置 splitter 的拉伸因子(左右比例为1：4)
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,3);

    // 添加传输列表/退出按钮
    addButton(CONFIG::PATH::TRANS, CONFIG::PATH::TRANS_HOVER);
    QPushButton* quitButton = addButton(CONFIG::PATH::QUIT, CONFIG::PATH::QUIT_HOVER);

    // 添加按钮之间的分割线
    addTitleLine(2);

    // 添加最大化，最小化按钮
    addMinmizeButton(CONFIG::PATH::MINIMIZE_PATH, CONFIG::PATH::MINIMIZE_HOVER_PATH);
    addMaxmizeButton(CONFIG::PATH::MAXIMIZE_PATH, CONFIG::PATH::MAXIMIZE_HOVER_PATH,
                     CONFIG::PATH::NORMAL_PATH, CONFIG::PATH::NORMAL_HOVER_PATH);

    // 设置窗口标题
    setTitleText("Qos Browser");

    // 设置窗口默认大小
    resize(1080, 640);

    //  关联信号与槽
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::loginSuccess, this, &UiMain::show);
    connect(quitButton, &QPushButton::clicked,MANAGER_GLOBAL->managerSignals, &ManagerSignals::unLogin);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::bucketsSuccess, this, &UiMain::onBucketsSuccess);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::objectsSuccess, this, &UiMain::onObjectsSuccess);
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
        connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::unLogin, this, &UiMain::showLoginDialog);
    }
    // 隐藏主窗口
    hide();
    m_loginDialog->show();
}

void UiMain::onBucketsSuccess(const QList<MyBucket>& buckets)
{
    Q_UNUSED(buckets);
    ui->stackedWidget->setCurrentIndex(0);
}

void UiMain::onObjectsSuccess(const QList<MyObject>& objects)
{
    Q_UNUSED(objects);
    ui->stackedWidget->setCurrentIndex(1);
}
