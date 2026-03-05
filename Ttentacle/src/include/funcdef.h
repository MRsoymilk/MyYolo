#ifndef FUNCDEF_H
#define FUNCDEF_H

// FUNCTION ===================================================================
#include <QDateTime>
#define TIMESTAMP(format) (QDateTime::currentDateTime().toString(format))

#include "../util/mysetting.h"
#define SETTING_INIT(path) MY_SETTING.init(path)
#define SETTING_GET(group, key, ...) MY_SETTING.getValue(group, key, ##__VA_ARGS__)
#define SETTING_SET(group, key, value) MY_SETTING.setValue(group, key, value)

#include "../util/mydir.h"
#define GET_ABSOLUTE_PATH(path) MyDir::GetAbsolutePath(path)

#include "../util/myprocess.h"
#define PROCESS_START_ATTACH(cmd, arguments) MY_PROCESS.startAttach(cmd, arguments)
#define PROCESS_START_DETACH(cmd, arguments) MY_PROCESS.startDetach(cmd, arguments)
#define PROCESS_STOP(t) MY_PROCESS.stopScript(t)

#define CHECK_AND_CREATE_DIR(root, dir_name)                 \
{                                                        \
        QString path = QString("%1/%2").arg(root, dir_name); \
        if (MyDir::CheckDirExists(path))                     \
    {                                                    \
            WIDGET_LOG_TRACE("exist dir " + path);           \
    }                                                    \
        else                                                 \
    {                                                    \
            WIDGET_LOG_WARN("no dir " + path);               \
            MyDir::CreateDir(path);                          \
    }                                                    \
}

#include <QMessageBox>
#define SHOW_MSGBOX_INFO(msg) QMessageBox::information(this, tr("Information"), msg);
#define SHOW_MSGBOX_WARNING(msg) QMessageBox::warning(this, tr("Warning"), msg);
#define SHOW_MSGBOX_CRITICAL(msg) QMessageBox::critical(this, tr("Critical"), msg);
#define SHOW_MSGBOX_INFO_REPLY(msg, opt) QMessageBox::information(this, tr("Information"), msg, opt);
#define SHOW_MSGBOX_WARNING_REPLY(msg, opt) QMessageBox::warning(this, tr("Warning"), msg, opt);
#define SHOW_MSGBOX_CRITICAL_REPLY(msg, opt) QMessageBox::critical(this, tr("Critical"), msg, opt);
#include <QFileDialog>
#include <QPushButton>

// open folder
#define OPEN_FOLDER(edit)                                                                 \
do {                                                                                      \
        QString folderPath = QFileDialog::getExistingDirectory(                               \
                                                                           this, "Open Folder", QString(),                                                   \
                                                                                                                                            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);                   \
        if (!folderPath.isEmpty()) {                                                          \
            (edit)->setText(folderPath);                                                      \
    }                                                                                     \
} while (0)


// open file
#define OPEN_FILE(edit)                                                                   \
    do {                                                                                     \
        QString filePath = QFileDialog::getOpenFileName(                                      \
                                                                             this, "Open File", QString(),                                                     \
                                                                             "All Files (*);;Text Files (*.txt);;Image Files (*.png *.jpg);;");               \
        if (!filePath.isEmpty()) {                                                            \
            (edit)->setText(filePath);                                                        \
    }                                                                                     \
} while (0)


// btn open folder
#define OPEN_FOLDER_BTN(btn, edit)                     \
do {                                                   \
        auto __edit = (edit);                              \
        QObject::connect((btn), &QPushButton::clicked,     \
                         this, [this, __edit]() {           \
                                 OPEN_FOLDER(__edit);                           \
                         });                                                \
} while (0)


// btn open file
#define OPEN_FILE_BTN(btn, edit)                       \
do {                                                   \
        auto __edit = (edit);                              \
        QObject::connect((btn), &QPushButton::clicked,     \
                         this, [this, __edit]() {           \
                                 OPEN_FILE(__edit);                             \
                         });                                                \
} while (0)
#endif
