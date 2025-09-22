#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>

/*
    该类仅用作展现屏蔽 ctrl + C ; ctrl + V ; ctrl + X 复制粘贴剪切功能的一个示例
    实际项目中使用事件过滤器来实现
*/


class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    MyLineEdit(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // MYLINEEDIT_H
