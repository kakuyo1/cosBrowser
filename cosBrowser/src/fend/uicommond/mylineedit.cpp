#include "mylineedit.h"
#include <QKeyEvent>

MyLineEdit::MyLineEdit(QWidget* parent) : QLineEdit(parent)
{}

void MyLineEdit::keyPressEvent(QKeyEvent *event)
{
    // 屏蔽ctrl + C ; ctrl + V ; ctrl + X
    if (event->modifiers() == Qt::ControlModifier) {
        if (event->key() == Qt::Key_C ||
            event->key() == Qt::Key_V ||
            event->key() == Qt::Key_X) {
            return;
        }
    }
    QLineEdit::keyPressEvent(event);
}
