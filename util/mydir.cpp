#include "mydir.h"

#include <QDir>
// #include <QStringEncoder>
// #include <QTextEncoder>

MyDir::MyDir() {}

bool MyDir::CheckDirExists(const QString &path)
{
    QDir dir(path);
    return dir.exists();
}

bool MyDir::CreateDir(const QString &path)
{
    QDir dir;
    return dir.mkpath(path);
}

// Qt 6.8
// std::string MyDir::Path2Path(const QString &path)
// {
//     QStringEncoder encoder = QStringEncoder(QStringConverter::Encoding::System);
//     QByteArray byteFilePath = encoder(path);
//     std::string stdFilePath = byteFilePath.toStdString();
//     return stdFilePath;
// }

std::string MyDir::Path2Path(const QString &path)
{
    QByteArray byteFilePath = path.toUtf8();
    std::string stdFilePath = byteFilePath.toStdString();
    return stdFilePath;
}
