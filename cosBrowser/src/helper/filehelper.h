#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QString>

class FileHelper
{
public:
    FileHelper();

    static QString readAllText(const QString& filePath);
    static QVariant readAllJson(const QString& filePath);

    static QString joinPath(const QString& path1, const QString& path2);

    static bool mkPath(const QString& path);

    static QList<QStringList> readAllCsv(const QString& filePath);

    static bool writeFile(const QStringList& rows, const QString& filePath);
};

#endif // FILEHELPER_H
