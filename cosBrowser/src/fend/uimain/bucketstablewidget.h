#ifndef BUCKETSTABLEWIDGET_H
#define BUCKETSTABLEWIDGET_H
#include "src/middle/models/mybucket.h"
#include <QWidget>

namespace Ui {
class BucketsTableWidget;
}

class BucketsTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BucketsTableWidget(QWidget *parent = nullptr);
    ~BucketsTableWidget();

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);
    void onPageNumberChanged(int startIndexInModel, int rowsToShow);
    void onBucketsSuccess(const QList<MyBucket>& buckets);
    void on_pushButtonCreateBucket_clicked();
    void onDeleteActionTriggered();
    void on_pushButtonRefresh_clicked();

private:
    Ui::BucketsTableWidget *ui;
};

#endif // BUCKETSTABLEWIDGET_H
