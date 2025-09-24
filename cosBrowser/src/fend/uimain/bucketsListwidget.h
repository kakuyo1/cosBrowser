#ifndef BUCKETSLISTWIDGET_H
#define BUCKETSLISTWIDGET_H

#include "src/middle/models/mybucket.h"
#include <QWidget>

namespace Ui {
class BucketsWidget;
}

class BucketsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BucketsWidget(QWidget *parent = nullptr);
    ~BucketsWidget();

private slots:
    void on_listViewBuckets_doubleClicked(const QModelIndex &index);
    void onBucketsSuccess(const QList<MyBucket>& buckets);
    void showBucketObjects(const QString& bucketName);
    void onTextChanged(const QString& text);
private:
    Ui::BucketsWidget *ui;
};

#endif // BUCKETSLISTWIDGET_H
