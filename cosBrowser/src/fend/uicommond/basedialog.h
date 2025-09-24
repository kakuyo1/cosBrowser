#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDialog>

namespace Ui {
class BaseDialog;
}

class BaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BaseDialog(QWidget *parent = nullptr);

    ~BaseDialog();

    // 设置标题文本
    void setTitleText(const QString& title);

    // 设置Logo
    void setLogoImage(const QString& imagePath);

    // 添加最小化按钮
    void addMinmizeButton(const QString& normalImagePth, const QString& hoverImagePath);

    // 添加最大化按钮
    /*
    * maxmizeImagePth: 最大化按钮图片路径
    * restoreImagePath: 恢复按钮图片路径
    */
    void addMaxmizeButton(const QString& maxmizeImagePth, const QString& hoverMaxmizeImagePath
                          , const QString& restoreImagePath, const QString& hoverRestoreImagePath);

    // 添加按钮
    QPushButton* addButton(const QString& normalImagePth, const QString& hoverImagePath);

    // 设置按钮大小
    void setAllButtonSize(int size);

    // 设置按钮图片
    void setButtonImage(QPushButton* button, const QString& normalImagePth, const QString& hoverImagePath);

    void setLogoFixedSize(int width, int height);
    void setTitleProperty(const QString& property, const QString& value);

    // 添加按钮之间的分割线
    void addTitleLine(int width = 2);
protected:
    // 重写鼠标按下事件
    void mousePressEvent(QMouseEvent* event) override;

    // 重写鼠标移动事件
    void mouseMoveEvent(QMouseEvent* event) override;

    // 重写事件过滤
    bool eventFilter(QObject* obj, QEvent* event) override;

    // 返回widgetBody
    QWidget* body();
private:
    // 添加关闭按钮
    void addCloseButton(const QString& normalImagePth, const QString& hoverImagePath);

    // 添加控件(如在关闭按钮前添加其他按钮)
    void addWidget(QWidget* w);

protected:
    Ui::BaseDialog *ui;
private:
    QPoint m_startPos; // 记录鼠标按下时的位置
    int m_buttonSize; // 按钮大小
};

#endif // BASEDIALOG_H
