#include "uiprogressbarwidget.h"
#include "ui_uiprogressbarwidget.h"

#include <src/helper/byteshelper.h>

UiProgressBarWidget::UiProgressBarWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UiProgressBarWidget)
{
    ui->setupUi(this);

    ui->progressBar->setStyleSheet(R"(
    QProgressBar {
        background:#edeffc;
    border-radius: 3px;
    }
    QProgressBar::chunk {
        background: #647bfc;
    border-radius: 3px;
    }
    )");
}

UiProgressBarWidget::~UiProgressBarWidget()
{
    delete ui;
}

void UiProgressBarWidget::setRange(qulonglong minValue, qulonglong maxValue)
{
    m_timer.restart();
    ui->progressBar->setRange(minValue, maxValue);
    ui->progressBar->setValue(0);
}

void UiProgressBarWidget::setValue(qulonglong value)
{
    double seconds = m_timer.elapsed() / 1000.0; // 获取经过的时间，单位为秒
    ui->progressBar->setValue(value);

    static qulonglong lastValue = 0; // 上一次更新的值
    qulonglong deltaValue = value - lastValue; // 计算增量
    lastValue = value; // 更新 lastValue 为当前值
    // 计算速度
    double speed = seconds > 0 ? deltaValue / seconds : 0; // 计算速度，单位为字节/秒
    QString speedText = BytesHelper::toHumanReadable(speed); // 转换为人类可读格式
    ui->labelSpeed->setText(speedText + "/s");
    // 计算剩余时间
    int leftValue = ui->progressBar->maximum() - value; // 剩余值
    if (speed > 0 && leftValue > 0) { // 避免除以零
        int leftTime = static_cast<int>(leftValue / speed); // 计算剩余时间，单位为秒
        ui->labelTime->setText(QString("剩余: %1s").arg(leftTime));

    } else {
        ui->labelTime->setText("");
    }
}

void UiProgressBarWidget::stop()
{
    m_timer.invalidate(); // invalidate() 用于使计时器无效，停止计时
    ui->labelSpeed->clear();
}

void UiProgressBarWidget::setFinished(const QString &message)
{
    ui->progressBar->setValue(ui->progressBar->maximum());
    ui->labelSpeed->setText(message);
    stop();
}

void UiProgressBarWidget::setError(const QString &message)
{
    stop();
    ui->labelSpeed->setText(message);
}
