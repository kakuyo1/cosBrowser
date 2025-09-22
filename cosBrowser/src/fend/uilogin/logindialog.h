#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class LoginDialog;
}
QT_END_NAMESPACE

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    void updateLoginInfo();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_btnClose_clicked();

    void on_btnLogin_clicked();

    void onLoginSuccess();
    void onError(int api, const QString& message, const QJsonValue& requestInfo);

private:
    Ui::LoginDialog *ui;
    QPoint m_startPos;
};
#endif // LOGINDIALOG_H
