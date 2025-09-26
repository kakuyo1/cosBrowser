#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <QDir>
#include "src/helper/filehelper.h"

namespace CONFIG {
// 程序相关路径
namespace PATH {
    static const QString WORK = QDir::currentPath();
    static const QString TMP = FileHelper::joinPath(WORK, "tmp");
    static const QString ERROE_CODE_PATH = ":/static/docs/errorcode.csv"; // 错误码CSV文件路径

    static const QString LOGO_PATH = ":/static/image/icontt.ico"; // 程序logo路径
    static const QString MAXIMIZE_PATH = ":/static/image/Maximize-1.png"; // 最大化按钮图片路径
    static const QString MAXIMIZE_HOVER_PATH = ":/static/image/Maximize-2.png"; // 最大化按钮hover图片路径
    static const QString NORMAL_PATH = ":/static/image/Maximize-3.png"; // 恢复按钮图片路径
    static const QString NORMAL_HOVER_PATH = ":/static/image/Maximize-4.png"; // 恢复按钮hover图片路径
    static const QString MINIMIZE_PATH = ":/static/image/Minimize-1.png"; // 最小化按钮图片路径
    static const QString MINIMIZE_HOVER_PATH = ":/static/image/Minimize-2.png"; // 最小化按钮hover图片路径

    static const QString QUIT = ":/static/image/Quit-1.png";
    static const QString QUIT_HOVER = ":/static/image/Quit-2.png";
    static const QString TRANS = ":/static/image/Trans-1.png";
    static const QString TRANS_HOVER = ":/static/image/Trans-2.png";

    static const QString BUCKET = ":/static/image/bucket.png";
    static const QString DIR = ":/static/image/dir.png";
    static const QString FILE = ":/static/image/file.png";
};
// SQLite数据库文件路径
namespace SQLITE {
    static const QString NAME = FileHelper::joinPath(PATH::TMP, "cos.db");
};
// 在程序第一次运行时利用代码创建login_info表，而是直接使用工具navicat
namespace SQL {
    static const QString LOGIN_INFO_TABLE = ":/static/sql/login_info.sql";
}
// 表名称
namespace TABLES {
    static const QString LOGIN_INFO = "login_info";
}

// 版本号
namespace VERSION {
    static const QString MAJOR_CUSTOM = "custom"; // custom主版本
    static const QString MAJOR_BUSINESS = "business"; // business主版本
    static const QString JSON_PATH = ":/static/version/config_default.json"; // 版本号配置文件路径
}

// 工作环境
namespace ENV {
    static const QString ENV_DEV = "dev"; // 开发环境
    static const QString ENV_ALPHA = "alpha"; // 测试环境
    static const QString ENV_BETA = "beta"; // 预发布环境
    static const QString ENV_PROD = "prod"; // 生产环境
    static const QString ENV_PRE = "pre"; // 准生产环境
}

static bool init() {
    return FileHelper::mkPath(PATH::TMP);
}

static bool OK = init(); // 启动程序自动创建必要的文件夹，在静态区
}


#endif // GLOBAL_H
