#ifndef BASEEXCEPTION_H
#define BASEEXCEPTION_H

#include <QMap>


using ErrorMap = QMap<QString, QString>;

class BaseException
{
public:
    BaseException(const QString& code, const QString& message);

    static ErrorMap parseErrorCode(const QString& csvPath); // 解析CSV文件，返回错误码映射

    static void generateErrorCodeHeadFile(const QString& csvPath, const QString& targetPath); // 生成错误码头文件


    QString message() const;
    QString code() const;

private:
    QString m_code;
    QString m_message;
};

#endif // BASEEXCEPTION_H
