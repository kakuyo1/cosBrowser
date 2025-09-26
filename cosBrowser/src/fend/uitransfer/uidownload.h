#ifndef UIDOWNLOAD_H
#define UIDOWNLOAD_H

#include <QWidget>

#include <src/fend/uicommond/uiprogressbarwidget.h>

namespace Ui {
class UiDownload;
}

class UiDownload : public QWidget
{
    Q_OBJECT

public:
    explicit UiDownload(QWidget *parent = nullptr);
    ~UiDownload();
    void onStartDownload(const QString &jobId, const QString &key, const QString &localPath, qulonglong totalSize);
    void onDownloadProgress(const QString& jobId, qulonglong transferred_size, qulonglong total_size);
    void onDownloadSuccess(const QString& jobId);
    void onError(int api, const QString& message, const QJsonValue& requestInfo);
private:
    UiProgressBarWidget* findTableWidgetByJobId(const QString& jobId);
    void initHeader();
private:
    Ui::UiDownload *ui;
    qulonglong maximumSize = 0; // 当前下载的文件的最大值
};

#endif // UIDOWNLOAD_H
