#ifndef MANAGERPLUGIN_H
#define MANAGERPLUGIN_H


#include <QObject>

#define MANAGER_PLUGIN ManagerPlugin::instance()

class DaoClouds;
class Version;
class ManagerPlugin : public QObject
{
    Q_OBJECT
public:
    explicit ManagerPlugin(QObject *parent = nullptr);
    ~ManagerPlugin();
    static ManagerPlugin* instance();
    DaoClouds *clouds() const;
    void installPlugins(int argc, char* argv[]);

signals:

private:


private:
    DaoClouds* m_clouds = nullptr; // 插件提供的云服务接口(虚基类)，可以替换为不同的实现(派生类)
    Version* m_version;
};

#endif // MANAGERPLUGIN_H
