#ifndef UIMAIN_H
#define UIMAIN_H

#include <QWidget>

#include <src/fend/uicommond/uiqoswidget.h>

#include <src/middle/models/mybucket.h>

namespace Ui {
class UiMain;
}

class LoginDialog;
class UiMain : public uiQosWidget
{
    Q_OBJECT

public:
    explicit UiMain(QWidget *parent = nullptr);
    ~UiMain();

    void showLoginDialog();

private slots:
    void onBucketsSuccess(const QList<MyBucket>& buckets);
    void onObjectsSuccess(const QList<MyObject>& objects);
private:
    Ui::UiMain *ui;
    LoginDialog *m_loginDialog;
};

#endif // UIMAIN_H
