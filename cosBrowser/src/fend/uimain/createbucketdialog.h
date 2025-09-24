#ifndef CREATEBUCKETDIALOG_H
#define CREATEBUCKETDIALOG_H
#include <src/fend/uicommond/uiqoswidget.h>
#include <QWidget>
#include <src/middle/models/mybucket.h>

namespace Ui {
class CreateBucketDialog;
}

class CreateBucketDialog : public uiQosWidget
{
    Q_OBJECT

public:
    explicit CreateBucketDialog(QWidget *parent = nullptr);
    ~CreateBucketDialog();

    MyBucket getBucket() const;
private:
    Ui::CreateBucketDialog *ui;
};

#endif // CREATEBUCKETDIALOG_H
