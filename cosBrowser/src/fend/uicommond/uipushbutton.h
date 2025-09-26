#ifndef UIPUSHBUTTON_H
#define UIPUSHBUTTON_H

#include <QEvent>
#include <QPushButton>

class UiPushButton : public QPushButton
{
    Q_OBJECT
public:
    UiPushButton(QWidget* parent = nullptr);
protected:
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
};

#endif // UIPUSHBUTTON_H
