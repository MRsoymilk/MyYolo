#ifndef DATADEF_H
#define DATADEF_H

#include <QString>

struct MY_GLOBAL
{
    QString PLATFORM = ""; // Windows / Linux
    QString PROEJCT = "";
    QString PYTHON = "";
    QString PIP = "";
    QString SCRIPT_TRAIN = "";
    QString SCRIPT_EXPORT = "";
    QString SCRIPT_MOVE_SIMILAR_IMG = "";
    QString SCRIPT_XML2TXT = "";
    QString SCRIPT_NO_XML2TXT = "";
    QString SCRIPT_RENAME = "";
    QString SCRIPT_RE_NOTAG = "";
    QString SCRIPT_SPLIT_DATASET = "";
    QString EXE_LABELIMG = "";
};

struct MSG_RE
{
    int code;
    QString msg;
    bool status;
};

#endif
