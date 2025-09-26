#include "uipushbutton.h"

UiPushButton::UiPushButton(QWidget* parent) : QPushButton(parent) {}

void UiPushButton::enterEvent(QEnterEvent* event)
{
    setCursor(Qt::PointingHandCursor);
    return QPushButton::enterEvent(event);
}

void UiPushButton::leaveEvent(QEvent *event)
{
    setCursor(Qt::ArrowCursor);
    return QPushButton::leaveEvent(event);
}
