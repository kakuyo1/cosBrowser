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
    : uiQosWidget(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(body());
    setTitleText("登录");
    // 设置logo图片(deprecated 改用QSS)
    // QPixmap logoPixmap("");
    // ui->labelLogo->setPixmap(logoPixmap.scaled(ui->labelLogo->size()));

    // QSS设置样式
    ui->labelSecretId->setProperty("style_font", "h5");
    ui->labelSecretKey->setProperty("style_font", "h5");
    ui->labelRemark->setProperty("style_font", "h5");
    ui->labelLoginName->setProperty("style_font", "h5");
    ui->btnLogin->setProperty("style_font", "h5");
    ui->btnLogin->setProperty("style_button", "main");

    // 安装事件过滤器
    ui->lineEditSecretKey->installEventFilter(this);

    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::loginSuccess,
            this, &LoginDialog::onLoginSuccess);
    connect(MANAGER_GLOBAL->managerSignals, &ManagerSignals::error,
            this, &LoginDialog::onError);

    connect(ui->btnLogin, &QPushButton::clicked, this, &LoginDialog::onBtnLoginClicked);

    updateLoginInfo();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::updateLoginInfo()
{
    QStringList words = MANAGER_GLOBAL->managerDataBase->loginNameList();
    ui->lineEditLoginName->setItems(words);

    connect(ui->lineEditLoginName,  &ComboLine::itemSelected,
            this, [=](const QString &name){
                LoginInfo info = MANAGER_GLOBAL->managerDataBase->loginInfoByName(name);
                ui->lineEditSecretId->setText(info.secret_id);
                ui->lineEditSecretKey->setText(info.secret_key);
                ui->lineEditRemark->setText(info.remark);
                ui->checkBoxSaveSection->setChecked(true);
            });
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

void LoginDialog::onBtnLoginClicked()
{
    if (ui->lineEditSecretId->text().trimmed().isEmpty() || ui->lineEditSecretKey->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "提示", "SecretId或SecretKet不能为空");
        return;
    }
    // 鉴权逻辑全部交给腾讯云执行
    QJsonObject params;
    params["secretId"] = ui->lineEditSecretId->text().trimmed();
    params["secretKey"] = ui->lineEditSecretKey->text().trimmed();
    GATE_WAY->send(API::LOGIN::NORMAL, params);
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

