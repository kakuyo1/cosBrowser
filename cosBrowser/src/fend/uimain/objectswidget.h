#ifndef OBJECTSWIDGET_H
#define OBJECTSWIDGET_H

#include <QWidget>

#include <src/middle/models/mybucket.h>

namespace Ui {
class ObjectsWidget;
}

class ObjectsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectsWidget(QWidget *parent = nullptr);
    ~ObjectsWidget();

private slots:
    void on_pushButtonBucketList_clicked();

    void on_tableViewObjects_doubleClicked(const QModelIndex &index);

    void onObjectsSuccess(const QList<MyObject>& objects);

    void onPathChanged(const QString& newPath);

    void onPageNumberChanged(int startIndexInModel, int rowsToShow);

    void on_pushButtonRefresh_clicked();

    void on_pushButtonUpload_clicked();

    void on_pushButtonDownload_clicked();

    void onUploadSuccess(const QString &jobId);

    void onDownloadSuccess(const QString &jobId);
private:
    void showMessageBox(const QString& title, const QString& messsage);

private:
    Ui::ObjectsWidget *ui;
};

#endif // OBJECTSWIDGET_H
