#ifndef DATADEF_H
#define DATADEF_H

#include <QString>

struct MY_GLOBAL
{
    QString PLATFORM = ""; // Windows / Linux
    QString PROEJCT = "";
    QString PYTHON = "";
    QString FILE_TRAIN = "";
    QString FILE_EXPORT = "";
};

struct MSG_RE
{
    int code;
    QString msg;
    bool status;
};

#endif
