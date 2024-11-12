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
