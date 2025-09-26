#ifndef UITRANSFER_H
#define UITRANSFER_H

#include <QWidget>
#include "../uicommond/uiqoswidget.h"
namespace Ui {
class UiTransfer;
}

class UiTransfer : public uiQosWidget
{
    Q_OBJECT

public:
    explicit UiTransfer(QWidget *parent = nullptr);
    ~UiTransfer();

private:
    Ui::UiTransfer *ui;
};

#endif // UITRANSFER_H
