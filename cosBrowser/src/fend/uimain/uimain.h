#ifndef UIMAIN_H
#define UIMAIN_H

#include <QWidget>

namespace Ui {
class UiMain;
}

class LoginDialog;
class UiMain : public QWidget
{
    Q_OBJECT

public:
    explicit UiMain(QWidget *parent = nullptr);
    ~UiMain();

    void showLoginDialog();

private:
    Ui::UiMain *ui;
    LoginDialog *m_loginDialog;
};

#endif // UIMAIN_H
