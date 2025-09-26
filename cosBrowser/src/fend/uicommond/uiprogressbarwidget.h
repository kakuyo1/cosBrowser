#ifndef UIPROGRESSBARWIDGET_H
#define UIPROGRESSBARWIDGET_H

#include <QElapsedTimer>
#include <QWidget>

namespace Ui {
class UiProgressBarWidget;
}

class UiProgressBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UiProgressBarWidget(QWidget *parent = nullptr);
    ~UiProgressBarWidget();

    void setRange(qulonglong minValue, qulonglong maxValue);
    void setValue(qulonglong value);
    void stop(); // 停止传输
    void setFinished(const QString& message);// 进度条满
    void setError(const QString& message);// 传输出错

private:
    Ui::UiProgressBarWidget *ui;
    // QElapsedTimer 提供了一个高精度的计时器，可以用来测量经过的时间间隔
    QElapsedTimer m_timer;
};

#endif // UIPROGRESSBARWIDGET_H
