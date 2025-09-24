#include "createbucketdialog.h"
#include "ui_createbucketdialog.h"

#include <QMessageBox>

CreateBucketDialog::CreateBucketDialog(QWidget *parent)
    : uiQosWidget(parent)
    , ui(new Ui::CreateBucketDialog)
{
    ui->setupUi(body());
    resize(300, 30);
    connect(ui->pushButtonConfirm, &QPushButton::clicked, this, &CreateBucketDialog::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &CreateBucketDialog::reject);
}

CreateBucketDialog::~CreateBucketDialog()
{
    delete ui;
}

MyBucket CreateBucketDialog::getBucket() const
{
    if (ui->lineEditLocation->text().trimmed().isEmpty() || ui->lineEditName->text().trimmed().isEmpty()) {
        QMessageBox::warning(const_cast<CreateBucketDialog*>(this), "警告", "存储桶名称和位置不能为空");
    }
    MyBucket bucket;
    bucket.name = ui->lineEditName->text().trimmed();
    bucket.location = ui->lineEditLocation->text().trimmed();
    return bucket;
}
