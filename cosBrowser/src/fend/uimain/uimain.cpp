#include "uimain.h"
#include "src/middle/managerglobal.h"
#include "ui_uimain.h"
#include "../uilogin/logindialog.h"
#include "src/config/global.h"
#include <src/middle/signals/managersignals.h>
#include <QPushButton>
#include <src/fend/uicommond/uimessagebox.h>
#include "src/config/apis.h"

UiMain::UiMain(QWidget *parent)
    : uiQosWidget(parent)
    , ui(new Ui::UiMain)
    , m_loginDialog(nullptr)
    , m_transferWidget(new UiTransfer(this))
{
    ui->setupUi(body());
    // 设置 splitter 的拉伸因子(左右比例为1：4)
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,3);

    // 添加传输列表/退出按钮
    QPushButton* transferButton = addButton(CONFIG::PATH::TRANS, CONFIG::PATH::TRANS_HOVER);
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

    // 样式
    ui->splitter->setStyleSheet("QSplitter::handle:horizontal { background: #dddddd;}");

    //  关联信号与槽
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::loginSuccess, this, &UiMain::show);
    connect(quitButton, &QPushButton::clicked,MANAGER_GLOBAL->managerSignals, &ManagerSignals::unLogin);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::bucketsSuccess, this, &UiMain::onBucketsSuccess);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::objectsSuccess, this, &UiMain::onObjectsSuccess);
    connect(transferButton, &QPushButton::clicked, this, &UiMain::showTransfer);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::error, this, &UiMain::onError);
}

UiMain::~UiMain()
{
    delete ui;
    if (m_loginDialog) {
        delete m_loginDialog;
        m_loginDialog = nullptr;
    }
    if (m_transferWidget) {
        delete m_transferWidget;
        m_transferWidget = nullptr;
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

void UiMain::showTransfer()
{
    m_transferWidget->show();
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

void UiMain::onError(int api, const QString &message, const QJsonValue &requestInfo)
{
    Q_UNUSED(requestInfo);
    // 不处理登录错误,只处理桶和对象错误
    if (api > API::BUCKETS::BASE) {
        UiMessageBox box;
        box.showMessage("错误", message);
    }
}
