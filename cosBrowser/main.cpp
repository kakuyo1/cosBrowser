#include "src/fend/uimain/uimain.h"
#include <QApplication>
#include <QMessageBox>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <src/middle/managerglobal.h>
#include <src/helper/filehelper.h>
#include <src/config/baseexception.h>
#include <src/config/global.h>
#include <src/bend/manager/managerdatabase.h>
#include <src/plugins/managerplugin.h>
#include <src/fend/uicommond/uiqoswidget.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    // 强制设置全局 locale
    setlocale(LC_ALL, "zh_CN.UTF-8");
    // 设置全局日志级别为debug
    spdlog::set_level(spdlog::level::debug);

    // 部分样式（qss中不生效）
    qApp->setStyleSheet(R"(/*设置滚动条*/
QScrollBar {
        border:none;
    background-color: transparent;
}

QScrollBar:horizontal {
        height: 6px;
}

QScrollBar:vertical {
        width: 6px;
}

QScrollBar::handle {
        background: #aaaaaa;
    border-radius: 3px;
}

QScrollBar::add-line, QScrollBar::sub-line {
        border: none;
}
)");

    // 初始化所有单例
    try{
        MANAGER_GLOBAL->init(argc, argv);
    }catch(const QString& error)
    {
        // 捕获异常并显示 QMessageBox
        QMessageBox::critical(nullptr, "插件初始化错误", error);
        return -1; // 返回非零值表示错误
    }

    UiMain w;
    w.showLoginDialog();
    return a.exec();
}
