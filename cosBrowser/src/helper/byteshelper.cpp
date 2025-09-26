#include "byteshelper.h"

QString BytesHelper::toHumanReadable(qulonglong bytes)
{
    if (bytes < 1024) {
        return QString::number(bytes) + " B";
    } else if (bytes < 1024 * 1024) {
        return QString::number(bytes / 1024.0, 'f', 2) + " KB";
    } else if (bytes < 1024 * 1024 * 1024) {
        return QString::number(bytes / (1024.0 * 1024), 'f', 2) + " MB";
    } else {
        return QString::number(bytes / (1024.0 * 1024 * 1024), 'f', 2) + " GB";
    }
}
