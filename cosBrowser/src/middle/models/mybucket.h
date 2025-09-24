#ifndef MYBUCKET_H
#define MYBUCKET_H

#include <QString>
#include <QMetaType>
#include <QList>

struct BaseObject {
    bool isValid() const { return !isInvalid();}
    bool isInvalid() const { return name.isNull() || name.isEmpty(); }
    QString name;
};
Q_DECLARE_METATYPE(BaseObject)            // 单个对象

struct MyBucket : public BaseObject
{
    MyBucket();
    QString location;
    QString createDate;
};

Q_DECLARE_METATYPE(MyBucket)              // 单个对象

struct MyObject : public BaseObject
{
    MyObject();
    bool isDir() const { return isValid() && name.endsWith('/'); };
    bool isFile() const { return isValid() && !name.endsWith('/'); };
    QString lastModified; // 最后修改时间
    qulonglong size = 0;
    QString dir; // 父目录
    QString key; // 对象键
};
Q_DECLARE_METATYPE(MyObject)              // 单个对象

#endif // MYBUCKET_H
