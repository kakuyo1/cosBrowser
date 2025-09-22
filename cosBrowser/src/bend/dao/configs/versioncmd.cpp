#include "versioncmd.h"

VersionCmd::VersionCmd(int argc, char* argv[]) {
    if (argc == 2) {
        QString args = argv[1];
        m_argv = args.split(".");
    }
}

void VersionCmd::setVersion()
{
    if (!isVersionValid()) {
        throw std::runtime_error("版本号格式错误，正确格式: business.alpha.1.0.0");
    }
    m_majorVersion = m_argv.at(0);
    m_environment = m_argv.at(1);
    m_serialNum1 = m_argv.at(2).toInt();
    m_serialNum2 = m_argv.at(3).toInt();
    m_serialNum3 = m_argv.at(4).toInt();
}

bool VersionCmd::isVersionValid()
{
    return m_argv.size() == 5;
}
