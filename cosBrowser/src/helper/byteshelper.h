#ifndef BYTESHELPER_H
#define BYTESHELPER_H
#include <QString>

class BytesHelper
{
public:
    BytesHelper() = default;

    static QString toHumanReadable(qulonglong bytes);
};

#endif // BYTESHELPER_H
