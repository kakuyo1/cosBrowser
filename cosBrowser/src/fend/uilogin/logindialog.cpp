#include "logindialog.h"
#include "src/bend/gateway.h"
#include "src/bend/manager/managerdatabase.h"
#include "ui_logindialog.h"
#include "src/config/apis.h"
#include <QCompleter>
#include <QJsonObject>
#include <QMessageBox>
#include <QMouseEvent>
#include "src/middle/managerglobal.h"
#include <src/middle/signals/managersignals.h>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
    , m_startPos()
{
    ui->setupUi(this);

    // 去掉标题栏
    setWindowFlags(Qt::CustomizeWindowHint);

    // 设置logo图片(deprecated 改用QSS)
    // QPixmap logoPixmap("");
    // ui->labelLogo->setPixmap(logoPixmap.scaled(ui->labelLogo->size()));

    // QSS设置样式
    ui->labelTitle->setProperty("TitleStyle", "h4");
    ui->labelSecretId->setProperty("TitleStyle", "h5");
    ui->labelSecretKey->setProperty("TitleStyle", "h5");
    ui->labelRemark->setProperty("TitleStyle", "h5");
    ui->labelLoginName->setProperty("TitleStyle", "h5");
    ui->btnClose->setProperty("TitleStyle", "h4");
    ui->btnLogin->setProperty("TitleStyle", "h5");

    // 安装事件过滤器
    ui->lineEditSecretKey->installEventFilter(this);

    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::loginSuccess,
            this, &LoginDialog::onLoginSuccess);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::error,
            this, &LoginDialog::onError);

    updateLoginInfo();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::updateLoginInfo()
{
    QStringList words = MANAGER_GLOBAL->managerDataBase->loginNameList();
    QCompleter* completer = new QCompleter(words);
    ui->lineEditLoginName->setCompleter(completer);

    connect(completer, static_cast<void (QCompleter::*)(const QString&)>(&QCompleter::activated),
            this, [this](const QString &name){
                LoginInfo info = MANAGER_GLOBAL->managerDataBase->loginInfoByName(name);
                ui->lineEditSecretId->setText(info.secret_id);
                ui->lineEditSecretKey->setText(info.secret_key);
                ui->lineEditRemark->setText(info.remark);
                ui->checkBoxSaveSection->setChecked(true);
            });
}

void LoginDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_startPos = event->pos(); // 记录鼠标按下时的位置(相對於父窗口)
    }
    QDialog::mousePressEvent(event);
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        if (event->pos().y() > 34) {// 标题栏高度 34, 仅允许在标题栏拖动窗口
            return;
        }
        QPoint targetPos = event->pos() - m_startPos + pos();
        /*
         event->pos 是鼠标当前位置(相對於父窗口)
         m_startPos 是鼠标按下时的位置(相對於父窗口)
         pos() 是窗口当前位置(相對於屏幕)
        */
        this->move(targetPos);
    }
    QDialog::mouseMoveEvent(event);
}

bool LoginDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->lineEditSecretKey) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            // 屏蔽ctrl + C ; ctrl + V ; ctrl + X
            if (keyEvent->modifiers() == Qt::ControlModifier) {
                if (keyEvent->key() == Qt::Key_C ||
                    keyEvent->key() == Qt::Key_V ||
                    keyEvent->key() == Qt::Key_X) {
                    return true; // 停止传递
                }
            }
        }
    }
    return QDialog::eventFilter(watched, event); // 继续传递
}

void LoginDialog::on_btnClose_clicked()
{
    reject();
}


void LoginDialog::on_btnLogin_clicked()
{
    QJsonObject params;
    params["secretId"] = ui->lineEditSecretId->text().trimmed();
    params["secretKey"] = ui->lineEditSecretKey->text().trimmed();
    GATE_WAY->send(API::LOGIN::NORMAL, params);

    // // 鉴权逻辑
    // if (ui->lineEditSecretId->text().trimmed() == "zhangsan" && // FAKE
    //     ui->lineEditSecretKey->text().trimmed() == "123")
    // {
    //     accept();
    //     if (ui->checkBoxSaveSection->isChecked()) {
    //         // 保存登录信息到数据库
    //         MANAGER_GLOBAL->managerDataBase->saveOrUpdateLoginInfo(
    //                     ui->lineEditLoginName->text().trimmed(),
    //                     ui->lineEditSecretId->text().trimmed(),
    //                     ui->lineEditSecretKey->text().trimmed(),
    //                     ui->lineEditRemark->text().trimmed());
    //     } else {
    //         // 删除登陆信息
    //         MANAGER_GLOBAL->managerDataBase->removeLoginInfo(ui->lineEditSecretId->text().trimmed());
    //     }
    //     updateLoginInfo(); // 更新登录信息
    // }
    // else
    // {
    //     QMessageBox::warning(this, "登录失败",
    //         "SecretId 或 SecretKey 错误，请重新输入！");
    // }
}

void LoginDialog::onLoginSuccess()
{
    if (ui->checkBoxSaveSection->isChecked()) {
        // 保存登录信息到数据库
        MANAGER_GLOBAL->managerDataBase->saveOrUpdateLoginInfo(
                    ui->lineEditLoginName->text().trimmed(),
                    ui->lineEditSecretId->text().trimmed(),
                    ui->lineEditSecretKey->text().trimmed(),
                    ui->lineEditRemark->text().trimmed());
    } else {
        // 删除登陆信息
        MANAGER_GLOBAL->managerDataBase->removeLoginInfo(ui->lineEditSecretId->text().trimmed());
    }
    accept(); // 关闭对话框并返回 QDialog::Accepted
}

void LoginDialog::onError(int api, const QString &message, const QJsonValue &requestInfo)
{
    Q_UNUSED(requestInfo);
    if (api != API::LOGIN::NORMAL) {
        return;
    }
    QMessageBox::warning(this, "登录失败", message);
}

