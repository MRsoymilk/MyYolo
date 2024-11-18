#include "mydir.h"

#include <QDir>

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

std::string MyDir::Path2Path(const QString &path)
{
    QStringEncoder encoder = QStringEncoder(QStringConverter::Encoding::System);
    QByteArray byteFilePath = encoder(path);
    std::string stdFilePath = byteFilePath.toStdString();
    return stdFilePath;
}
