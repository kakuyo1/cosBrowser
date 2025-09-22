#include "managersignals.h"

ManagerSignals::ManagerSignals(QObject *parent)
    : QObject{parent}
{
    // 注册第三方特殊类型，使之可以在信号槽中传递
    qRegisterMetaType<MyBucket>("MyBucket");
    qRegisterMetaType<QList<MyBucket>>("QList<MyBucket>"); // 注册容器在运行时
}

ManagerSignals::~ManagerSignals()
{

}
