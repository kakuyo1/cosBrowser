#ifndef VERSION_H
#define VERSION_H

#include <QString>
#include <QJsonObject>

class Version
{
public:
    Version();
    virtual void setVersion() = 0;

    QString version();
    QString versionSerialNumber();
    QString majorVersion();

protected:
    QString m_majorVersion; // as like custom/business
    QString m_environment; // as like dev/test/prod

    // version number 1.0.0
    int m_serialNum1; // 1
    int m_serialNum2; // 0
    int m_serialNum3; // 0
};

#endif // VERSION_H
