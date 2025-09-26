#include "uimessagebox.h"
#include "ui_uimessagebox.h"
#include "uipushbutton.h"

#include <QPushButton>

UiMessageBox::UiMessageBox(QWidget *parent)
    : uiQosWidget(parent)
    , ui(new Ui::UiMessageBox)
{
    ui->setupUi(body());
    resize(350,50);
}

UiMessageBox::~UiMessageBox()
{
    delete ui;
}

QString UiMessageBox::showMessage(const QString &title, const QString &message,
                                  const QStringList &buttonTextList,const int width, const int height)
{
    // 清空之前的内容
    m_text.clear();
    resize(width, height);
    setTitleText(title);
    ui->label->setText(message);
    // 创建按钮
    createButtons(buttonTextList);
    exec(); // 模态显示,等待用户操作
    return m_text;  // 似乎没用
}

void UiMessageBox::createButtons(const QStringList &textList)
{
    for (const auto& text : std::as_const(textList)) {
        QPushButton* button = new UiPushButton(this);
        button->setText(text);
        connect(button, &QPushButton::clicked, this, [=](){
            m_text = text;
            accept();
        });
        ui->horizontalLayout->addWidget(button);
    }
}
