#include "basedialog.h"
#include "ui_basedialog.h"

#include <QMouseEvent>

BaseDialog::BaseDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BaseDialog)
    , m_startPos(QPoint(0,0))
    , m_buttonSize(20)
{
    ui->setupUi(this);
    // 去掉标题栏
    setWindowFlags(Qt::CustomizeWindowHint | Qt::Window);
    // 添加关闭按钮
    addCloseButton(":/static/image/close_normal_.png", ":/static/image/close_hover_.png");
    // 设置默认按钮大小
    setAllButtonSize(m_buttonSize);
    // 设置图标
    setLogoImage(":/static/image/icontt.ico");
    ui->labelLogo->setFixedSize(40, 40);
    ui->frameTitle->setStyleSheet("QFrame{ border: none}");
}

BaseDialog::~BaseDialog()
{
    delete ui;
}

void BaseDialog::setTitleText(const QString &title)
{
    ui->labelTitle->setText(title);
}

void BaseDialog::setLogoImage(const QString &imagePath)
{
    ui->labelLogo->setStyleSheet(QString("border-image:url(\"%1\")").arg(imagePath));
}

void BaseDialog::addMinmizeButton(const QString &normalImagePth, const QString &hoverImagePath)
{
    QPushButton* button = addButton(normalImagePth, hoverImagePath);
    connect(button, &QPushButton::clicked, this, &BaseDialog::showMinimized);
}

void BaseDialog::addMaxmizeButton(const QString &maxmizeImagePth, const QString &hoverMaxmizeImagePath, const QString &restoreImagePath, const QString &hoverRestoreImagePath)
{
    QPushButton* button = addButton(maxmizeImagePth, hoverMaxmizeImagePath);
    auto funcImage = [=](){
        isMaximized() ?
                    setButtonImage(button, restoreImagePath, hoverRestoreImagePath) :
                    setButtonImage(button, maxmizeImagePth, hoverMaxmizeImagePath);
    };
    auto funcShow = [=](){
        isMaximized() ? showNormal() : showMaximized();
        funcImage();
    };
    connect(button, &QPushButton::clicked, this, funcShow);
    funcImage(); // 初始化按钮图片
}

QPushButton *BaseDialog::addButton(const QString &normalImagePth, const QString &hoverImagePath)
{
    QPushButton* button = new UiPushButton;
    button->setFixedSize(m_buttonSize, m_buttonSize);
    setButtonImage(button, normalImagePth, hoverImagePath);
    addWidget(button);
    return button;
}

void BaseDialog::setAllButtonSize(int size)
{
    QList<QPushButton*> buttons = ui->frameTitle->findChildren<QPushButton*>();
    for (QPushButton* button : std::as_const(buttons)) {
        button->setFixedSize(size, size);
    }
    m_buttonSize = size;
}

void BaseDialog::setButtonImage(QPushButton *button, const QString &normalImagePth, const QString &hoverImagePath)
{
    button->setStyleSheet(QString("QPushButton{border-image: url(\"%1\");}"
                                  "QPushButton:hover{border-image:url(\"%2\");}").arg(normalImagePth, hoverImagePath));
}

void BaseDialog::setLogoFixedSize(int width, int height)
{
    ui->labelLogo->setFixedSize(width, height);
}

void BaseDialog::setTitleProperty(const QString &property, const QString &value)
{
    ui->labelTitle->setProperty(property.toUtf8().constData(), value);
}

void BaseDialog::addTitleLine(int width)
{
    QLabel* line = new QLabel;
    line->setFixedSize(width, 15);
    line->setStyleSheet("QLabel{background-color: lightgray;}");
    addWidget(line);
}

void BaseDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_startPos = event->pos(); // 记录鼠标按下时的位置(相對於父窗口)
    }
    QDialog::mousePressEvent(event);
}

void BaseDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        if (event->pos().y() > 50) {// 标题栏高度 50, 仅允许在标题栏拖动窗口
            return;
        }
        QPoint targetPos = event->pos() - m_startPos + pos();
        /*
         event->pos 是鼠标当前位置(相對於父窗口)
         m_startPos 是鼠标按下时的位置(相對於父窗口)
         pos() 是窗口当前位置(相對於屏幕)
        */
        this->move(targetPos);
    }
    QDialog::mouseMoveEvent(event);
}

bool BaseDialog::eventFilter(QObject *obj, QEvent *event)
{
    // 仅对 BaseDialog 及其子类起作用
    BaseDialog* watched = qobject_cast<BaseDialog*>(obj);
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            // 屏蔽Enter Esc Return,作用：阻止对话框关闭, 必须通过按钮关闭
                if (keyEvent->key() == Qt::Key_Return ||
                    keyEvent->key() == Qt::Key_Escape ||
                    keyEvent->key() == Qt::Key_Enter) {
                    return true; // 停止传递
            }
        }
    return QDialog::eventFilter(watched, event); // 继续传递
}

QWidget *BaseDialog::body()
{
    return ui->widgetBody;
}

void BaseDialog::addCloseButton(const QString &normalImagePth, const QString &hoverImagePath)
{
    // 关闭按钮一定要第一个添加
    setButtonImage(ui->pushButtonClose, normalImagePth, hoverImagePath);
    ui->horizontalLayout->addWidget(ui->pushButtonClose);
    connect(ui->pushButtonClose, &QPushButton::clicked, this, &BaseDialog::close);
}

void BaseDialog::addWidget(QWidget *w)
{
    int i = ui->horizontalLayout->indexOf(ui->pushButtonClose); // 找到关闭按钮的位置
    ui->horizontalLayout->insertWidget(i, w); // 插入到关闭按钮前面
}
