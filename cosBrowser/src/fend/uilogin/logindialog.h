#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

#include <src/fend/uicommond/uiqoswidget.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class LoginDialog;
}
QT_END_NAMESPACE

class LoginDialog : public uiQosWidget
{
    Q_OBJECT

public:
    LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    void updateLoginInfo();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onBtnLoginClicked();

    void onLoginSuccess();
    void onError(int api, const QString& message, const QJsonValue& requestInfo);

private:
    Ui::LoginDialog *ui;
};
#endif // LOGINDIALOG_H
