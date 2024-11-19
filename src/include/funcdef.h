#ifndef FUNCDEF_H
#define FUNCDEF_H

// FUNCTION ===================================================================

#include <QString>
#define TXT_INFO(msg) ui->txtEditLog->append(QString("[INFO] %1").arg(msg))
#define TXT_WARN(msg) ui->txtEditLog->append(QString("[WARN] %1").arg(msg))

#define CHECK_AND_CREATE_DIR(root, dir_name) \
    { \
        QString path = QString("%1/%2").arg(root, dir_name); \
        if (MyDir::CheckDirExists(path)) { \
            TXT_INFO("exist dir " + path); \
        } else { \
            TXT_WARN("no dir " + path); \
            MyDir::CreateDir(path); \
        } \
    }

#include <QMessageBox>
#define SHOW_MSGBOX_INFO(msg) QMessageBox::information(this, "Information", msg);
#define SHOW_MSGBOX_WARNING(msg) QMessageBox::warning(this, "Warning", msg);
#define SHOW_MSGBOX_CRITICAL(msg) QMessageBox::critical(this, "Critical", msg);

#include <QFileDialog>
// open folder
#define OPEN_FOLDER(edit) \
    { \
        QString folderPath \
            = QFileDialog::getExistingDirectory(this, \
                                                "Open Folder", \
                                                QString(), \
                                                QFileDialog::ShowDirsOnly \
                                                    | QFileDialog::DontResolveSymlinks); \
        if (!folderPath.isEmpty()) { \
            edit->setText(folderPath); \
        } \
    }

// open file
#define OPEN_FILE(edit) \
    { \
        QString filePath = QFileDialog::getOpenFileName( \
            this, \
            "Open File", \
            QString(), \
            "All Files (*);;Text Files (*.txt);;Image Files (*.png *.jpg);;"); \
        if (!filePath.isEmpty()) { \
            edit->setText(filePath); \
        } \
    }

#include <QPushButton>
// btn open folder
#define OPEN_FOLDER_BTN(btn, edit) \
    QObject::connect(btn, &QPushButton::clicked, this, [this]() { OPEN_FOLDER(edit); })

// btn open file
#define OPEN_FILE_BTN(btn, edit) \
    QObject::connect(btn, &QPushButton::clicked, this, [this]() { OPEN_FILE(edit); })

// FUNCTION ===================================================================

#endif
