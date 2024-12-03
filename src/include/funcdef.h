#ifndef FUNCDEF_H
#define FUNCDEF_H

// FUNCTION ===================================================================
#include <QDateTime>
#define TIMESTAMP(format) (QDateTime::currentDateTime().toString(format))

#include "../util/mylog.h"
#define LOG_TRACE(...) MY_LOG.getLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) MY_LOG.getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) MY_LOG.getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) MY_LOG.getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) MY_LOG.getLogger()->critical(__VA_ARGS__)

#include "../util/mysetting.h"
#define SETTING_GET(group, key, ...) \
  MY_SETTING.getValue(group, key, ##__VA_ARGS__)
#define SETTING_SET(group, key, value) MY_SETTING.setValue(group, key, value)

#include "../WidgetLog/widgetlog.h"
#define WIDGET_LOG_TRACE(msg) MY_WIDGET_LOG.logTrace(msg)
#define WIDGET_LOG_INFO(msg) MY_WIDGET_LOG.logInfo(msg)
#define WIDGET_LOG_WARN(msg) MY_WIDGET_LOG.logWarn(msg)
#define WIDGET_LOG_ERROR(msg) MY_WIDGET_LOG.logError(msg)
#define WIDGET_LOG_CLOSE() MY_WIDGET_LOG.close()

#include "../util/mydir.h"
#define GET_ABSOLUTE_PATH(path) MyDir::GetAbsolutePath(path)

#include "../util/myprocess.h"
#define PROCESS_START_ATTACH(cmd, arguments) \
  MY_PROCESS.startAttach(cmd, arguments)
#define PROCESS_START_DETACH(cmd, arguments) \
  MY_PROCESS.startDetach(cmd, arguments)
#define PROCESS_STOP(t) MY_PROCESS.stopScript(t)

#define CHECK_AND_CREATE_DIR(root, dir_name)             \
  {                                                      \
    QString path = QString("%1/%2").arg(root, dir_name); \
    if (MyDir::CheckDirExists(path)) {                   \
      WIDGET_LOG_TRACE("exist dir " + path);             \
    } else {                                             \
      WIDGET_LOG_WARN("no dir " + path);                 \
      MyDir::CreateDir(path);                            \
    }                                                    \
  }

#include <QMessageBox>
#define SHOW_MSGBOX_INFO(msg) \
  QMessageBox::information(this, tr("Information"), msg);
#define SHOW_MSGBOX_WARNING(msg) QMessageBox::warning(this, tr("Warning"), msg);
#define SHOW_MSGBOX_CRITICAL(msg) \
  QMessageBox::critical(this, tr("Critical"), msg);
#define SHOW_MSGBOX_INFO_REPLY(msg, opt) \
  QMessageBox::information(this, tr("Information"), msg, opt);
#define SHOW_MSGBOX_WARNING_REPLY(msg, opt) \
  QMessageBox::warning(this, tr("Warning"), msg, opt);
#define SHOW_MSGBOX_CRITICAL_REPLY(msg, opt) \
  QMessageBox::critical(this, tr("Critical"), msg, opt);

#include <QFileDialog>
// open folder
#define OPEN_FOLDER(edit)                                              \
  {                                                                    \
    QString folderPath = QFileDialog::getExistingDirectory(            \
        this, "Open Folder", QString(),                                \
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks); \
    if (!folderPath.isEmpty()) {                                       \
      edit->setText(folderPath);                                       \
    }                                                                  \
  }

// open file
#define OPEN_FILE(edit)                                                    \
  {                                                                        \
    QString filePath = QFileDialog::getOpenFileName(                       \
        this, "Open File", QString(),                                      \
        "All Files (*);;Text Files (*.txt);;Image Files (*.png *.jpg);;"); \
    if (!filePath.isEmpty()) {                                             \
      edit->setText(filePath);                                             \
    }                                                                      \
  }

#include <QPushButton>
// btn open folder
#define OPEN_FOLDER_BTN(btn, edit)                   \
  QObject::connect(btn, &QPushButton::clicked, this, \
                   [this]() { OPEN_FOLDER(edit); })

// btn open file
#define OPEN_FILE_BTN(btn, edit)                     \
  QObject::connect(btn, &QPushButton::clicked, this, \
                   [this]() { OPEN_FILE(edit); })
// FUNCTION ===================================================================

#endif
