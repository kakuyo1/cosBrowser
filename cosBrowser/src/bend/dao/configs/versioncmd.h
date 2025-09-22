#ifndef VERSIONCMD_H
#define VERSIONCMD_H

#include "version.h"

class VersionCmd : public Version
{
public:
    VersionCmd(int argc, char* argv[]);
    virtual void setVersion() override;
    bool isVersionValid();
private:
    QStringList m_argv;
};

#endif // VERSIONCMD_H
