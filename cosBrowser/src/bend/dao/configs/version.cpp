#include "version.h"

Version::Version() {}

QString Version::version()
{
    return QString("%1.%2.%3").arg(m_majorVersion, m_environment, versionSerialNumber());
}

QString Version::versionSerialNumber()
{
    return QString("%1.%2.%3").arg(m_serialNum1, m_serialNum2, m_serialNum3);
}

QString Version::majorVersion()
{
    return m_majorVersion;
}
