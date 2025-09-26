#ifndef UIUPLOAD_H
#define UIUPLOAD_H

#include <QWidget>

#include <src/fend/uicommond/uiprogressbarwidget.h>

namespace Ui {
class UiUpload;
}

class UiUpload : public QWidget
{
    Q_OBJECT

public:
    explicit UiUpload(QWidget *parent = nullptr);
    ~UiUpload();

private slots:
    void onStartUpload(const QString& jobId, const QString& key, const QString& localPath);
    void onUploadProgress(const QString& jobId, qulonglong transferred_size, qulonglong total_size);
    void onUploadSuccess(const QString& jobId);
    void onError(int api, const QString& message, const QJsonValue& requestInfo);
private:
    UiProgressBarWidget* findTableWidgetByJobId(const QString& jobId);
    void initHeader();
    Ui::UiUpload *ui;
};

#endif // UIUPLOAD_H
