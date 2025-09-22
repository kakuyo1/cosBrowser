#include "versionjson.h"

#include <src/helper/filehelper.h>

VersionJson::VersionJson(const QString& path) {
    m_path = path;
}

void VersionJson::setVersion()
{
    QJsonObject obj = FileHelper::readAllJson(m_path).toJsonObject();
    obj = obj["version"].toObject();
    m_majorVersion = obj["major"].toString();
    m_environment = obj["env"].toString();
    m_serialNum1 = obj["v1"].toInt();
    m_serialNum2 = obj["v2"].toInt();
    m_serialNum3 = obj["v3"].toInt();
}
