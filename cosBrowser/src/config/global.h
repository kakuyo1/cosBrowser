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
