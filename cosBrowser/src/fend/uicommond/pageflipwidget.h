#ifndef PAGEFLIPWIDGET_H
#define PAGEFLIPWIDGET_H

#include <QWidget>

namespace Ui {
class PageFlipWidget;
}

class PageFlipWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PageFlipWidget(QWidget *parent = nullptr);
    ~PageFlipWidget();

private:
    Ui::PageFlipWidget *ui;
};

#endif // PAGEFLIPWIDGET_H
