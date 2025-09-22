#ifndef MANAGERGLOBAL_H
#define MANAGERGLOBAL_H

#include <QObject>

#define MANAGER_GLOBAL ManagerGlobal::instance()

class GateWay;
class ManagerSignals;
class ManagerPlugin;
class ManagerDataBase;
class ManagerCloud;
class ManagerModels;
class ManagerGlobal : public QObject
{
    Q_OBJECT
public:
    explicit ManagerGlobal(QObject *parent = nullptr);
    ~ManagerGlobal();
    static ManagerGlobal* instance();
    void init(int argc, char *argv[]);
public:
    GateWay* gateway = nullptr;
    ManagerSignals* managerSignals = nullptr;
    ManagerPlugin* managerPlugin = nullptr;
    ManagerDataBase* managerDataBase = nullptr;
    ManagerCloud* managerCloud = nullptr;
    ManagerModels* managerModels = nullptr;
};

#endif // MANAGERGLOBAL_H
