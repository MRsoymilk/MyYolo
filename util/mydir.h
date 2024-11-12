#ifndef MYDIR_H
#define MYDIR_H

#include <QString>

class MyDir
{
public:
    MyDir();
    static bool CheckDirExists(const QString& path);
    static bool CreateDir(const QString& path);
};

#endif // MYDIR_H
