#ifndef UIMAIN_H
#define UIMAIN_H

#include <QWidget>

#include <src/fend/uicommond/uiqoswidget.h>

#include <src/middle/models/mybucket.h>

#include <src/fend/uitransfer/uitransfer.h>

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

    void showTransfer();
private slots:
    void onBucketsSuccess(const QList<MyBucket>& buckets);
    void onObjectsSuccess(const QList<MyObject>& objects);
    void onError(int api, const QString& message, const QJsonValue& requestInfo);
private:
    Ui::UiMain *ui;
    LoginDialog *m_loginDialog;
    UiTransfer* m_transferWidget;
};

#endif // UIMAIN_H
