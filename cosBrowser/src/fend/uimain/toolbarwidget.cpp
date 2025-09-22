#include "toolbarwidget.h"
#include "ui_toolbarwidget.h"

#include <QFileDialog>

ToolBarWidget::ToolBarWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ToolBarWidget)
{
    ui->setupUi(this);

    // 綁定退出登录按钮
    connect(ui->btnLoginOut, &QPushButton::clicked,this, &ToolBarWidget::quitLogin);

}

ToolBarWidget::~ToolBarWidget()
{
    delete ui;
}

void ToolBarWidget::on_btnUpLoad_clicked()
{
    // 1.上傳單個文件
    // QString path = QFileDialog::getOpenFileName(this, "选择文件上传", "./");
    // qDebug() << path;

    // 2.上傳多個文件
    // QStringList paths = QFileDialog::getOpenFileNames(this, "选择文件上传", "./");
    // qDebug() << paths;

    // 3.上傳指定類型的多個文件
    // QStringList paths = QFileDialog::getOpenFileNames(
    //     this,
    //     "选择文件上传",
    //     "./",
    //     "hpp (*.h *.cpp);; MakeFile (*.Release *.Debug)");
    // qDebug() << paths;

    // 4.上傳文件夾
    // QString path = QFileDialog::getExistingDirectory(this, "选择文件夹上传", "./");
    // qDebug() << path;
}


void ToolBarWidget::on_btnDownLoad_clicked()
{
    // 1. 保存文件
    // QString path = QFileDialog::getSaveFileName(this, "选择文件保存路径", "./");
    // qDebug() << path;

    // 2.指定類型的保存文件
    // QString path = QFileDialog::getSaveFileName(this, "选择文件保存路径", "./",
    //  "hpp (*.h *.cpp);; MakeFile (*.Release *.Debug)");
}

