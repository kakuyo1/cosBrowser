#include "baseexception.h"

#include <src/helper/filehelper.h>

#include "src/config/global.h"

BaseException::BaseException(const QString &code, const QString &message)
    : m_code(code), m_message(message)
{

}

ErrorMap BaseException::parseErrorCode(const QString &csvPath)
{
    ErrorMap errorMap;
    int default_sectionCount = 6; // 默认的分段数量(在沒有错误详细说明的情况下)
    QList<QStringList> records = FileHelper::readAllCsv(csvPath);

    for (int i = 0; i < records.size(); ++i) {
        auto record = records.at(i);

        QString code = "";
        QString message = "";
        for (int j = 0; j < default_sectionCount; ++j) {
            if (record[j] == "") { // 如果某一段为空，则补0
                code += "0";
            } else {
                QStringList strs = record[j].split("_");
                message += strs[0];
                code += strs[1];
            }
        }
        if (record[default_sectionCount] != "") { // 如果有错误详细说明（写在文件里面的详细说明，不是开发者在代码中写的）
            message = record[default_sectionCount];  // 详细说明优先级高于自动生成的说明，前者覆盖后者
        }
        errorMap.insert(code, message);
    }
    return errorMap;
}
void BaseException::generateErrorCodeHeadFile(const QString &csvPath,
                                              const QString &targetPath){
    QStringList rows;
    rows << "#ifndef ERRORCODE_H" << "#define ERRORCODE_H\n\n";

    ErrorMap errorMap = parseErrorCode(csvPath);
    ErrorMap::const_iterator iter = errorMap.constBegin();
    while (iter != errorMap.constEnd()) {
        // 添加宏
        rows << QString("#define EC_%1 \"%1\"  // %2").arg(iter.key(), iter.value());
        ++iter;
    }
    rows << "\n\n#endif // ERRORCODE_H";
    FileHelper::writeFile(rows, targetPath);
}
QString BaseException::message() const {
    // 使用static确保只解析一次csv
    static ErrorMap errorMap = parseErrorCode(CONFIG::PATH::ERROE_CODE_PATH);
    // 格式化错误信息
    QString message = QString("错误 (%1) : %2").arg(m_code, errorMap[m_code]);
    // 如果有自定义的错误信息(开发者在代码中写的)，则添加到后面（可选）
    if (!m_message.isEmpty()) {
        message += QString(",  详情: %1").arg(m_message);
    }
    return message;
}

QString BaseException::code() const
{
    return m_code;
}
