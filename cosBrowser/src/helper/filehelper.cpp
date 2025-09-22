#include "filehelper.h"

#include <QFile>
#include <QJsonDocument>
#include <QVariant>
#include <QDir>
#include <QRegularExpression>

FileHelper::FileHelper() {}

QString FileHelper::readAllText(const QString &filePath)
{
    QFile file (filePath);
    if (file.exists()&& file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();
        return QString(data);
    }
    throw std::runtime_error("打开文件失败");
}

QVariant FileHelper::readAllJson(const QString &filePath)
{
    QString data = readAllText(filePath);
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    return doc.toVariant();
}

QString FileHelper::joinPath(const QString &path1, const QString &path2)
{
    QString path = path1 + "/" + path2;
    QStringList pathList = path.split(QRegularExpression("[/\\\\]"), Qt::SkipEmptyParts); // 处理多余的斜杠
    path = pathList.join("/"); // 统一使用正斜杠
    return QDir::cleanPath(path); // 规范化路径,例如去掉多余的.和..
}

bool FileHelper::mkPath(const QString &path)
{
    QDir dir;
    return dir.mkpath(path);
}

QList<QStringList> FileHelper::readAllCsv(const QString &filePath)
{
    QList<QStringList> records;
    QFile file (filePath);
    if (file.exists()&& file.open(QIODevice::ReadOnly)) {
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            QStringList fields = QString(line).split(",");
            records.append(fields);
        }
        file.close();
        return records;
    }
    throw std::runtime_error("打开文件失败");
}

bool FileHelper::writeFile(const QStringList &rows, const QString &filePath)
{
    QFile file (filePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        for (const QString& row : rows) {
            out << row << "\n";
        }
        file.close();
        return true;
    }
    throw std::runtime_error("写入文件失败");
}
