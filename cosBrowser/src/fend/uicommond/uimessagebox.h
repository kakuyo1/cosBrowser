#ifndef UIMESSAGEBOX_H
#define UIMESSAGEBOX_H

#include "uiqoswidget.h"

#include <QWidget>

namespace Ui {
class UiMessageBox;
}

class UiMessageBox : public uiQosWidget
{
    Q_OBJECT
public:
    explicit UiMessageBox(QWidget *parent = nullptr);
    ~UiMessageBox();
    QString showMessage(const QString& title, const QString& message,
                    const QStringList& buttonTextList = QStringList("确定"), const int width = 400, const int height = 50);
private:
    void createButtons(const QStringList& textList);
private:
    Ui::UiMessageBox *ui;
    QString m_text; // 作用：
};

#endif // UIMESSAGEBOX_H
