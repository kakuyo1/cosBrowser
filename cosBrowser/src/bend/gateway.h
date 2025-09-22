#ifndef GATEWAY_H
#define GATEWAY_H

#include <QObject>

#define GATE_WAY GateWay::instance()

class GateWay : public QObject
{
    Q_OBJECT
public:
    explicit GateWay(QObject *parent = nullptr);
    static GateWay* instance();
    void send(int api, const QJsonValue& value);
private:
    // API 路由分发
    void dispatch(int api, const QJsonValue& value);
    // 登陆
    void apiLogin(const QJsonValue& value);
    // 桶操作
    void apiBucketsList(const QJsonValue& value);
    void apiBucketsPut(const QJsonValue& value);
    void apiBucketsDelete(const QJsonValue& value);
    // 对象操作
    void apiObjectsList(const QJsonValue& value);
    void apiObjectsGet(const QJsonValue& value);
    void apiObjectsPut(const QJsonValue& value);
signals:
};

#endif // GATEWAY_H
