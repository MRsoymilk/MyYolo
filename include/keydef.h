#ifndef KEYDEF_H
#define KEYDEF_H

// config file
#define CFG_GROUP_ADD_DATA          "ADD_DATA"
#define CFG_ADD_DATA_DIR_INPUT      "dir_input"
#define CFG_ADD_DATA_DIR_OUTPUT     "dir_output"
#define CFG_ADD_DATA_TAG            "tag"
#define CFG_ADD_DATA_SCRIPT         "script"
#define CFG_ADD_DATA_DIR_TODO       "dir_todo"
#define CFG_ADD_DATA_TRAIN          "train"
#define CFG_ADD_DATA_VALID          "valid"
#define CFG_ADD_DATA_TEST           "test"
#define CFG_GROUP_TRAIN             "TRAIN"
#define CFG_TRAIN_MODEL             "model"
#define CFG_TRAIN_WEIGHTS           "weights"
#define CFG_TRAIN_DATA              "data"
#define CFG_TRAIN_OUTPUT            "output"
#define CFG_TRAIN_HYP               "hyp"
#define CFG_TRAIN_EPOCH             "epoch"
#define CFG_TRAIN_BATCH_SIZE        "batch_size"
#define CFG_TRAIN_IMG_SIZE          "img_size"
#define CFG_GROUP_TEST              "TEST"
#define CFG_TEST_MODEL              "model"
#define CFG_TEST_MODEL_HEIGHT       "model_height"
#define CFG_TEST_MODEL_WIDTH        "model_width"
#define CFG_TEST_DIR_INPUT          "dir_input"
#define CFG_TEST_DIR_OUTPUT         "dir_output"
#define CFG_GROUP_MAIN              "MAIN"
#define CFG_MAIN_PROJECT_DIR        "project_dir"
#define CFG_GROUP_EXPORT            "EXPORT"
#define CFG_EXPORT_TYPE             "type"
#define CFG_EXPORT_PT_MODEL         "pt_model"
#define CFG_EXPORT_DIR_OUTPUT       "dir_output"
// dir
#define DIR_TEST                    "test"
#define DIR_MODEL                   "model"
#define DIR_TOOL                    "tool"
#define DIR_SCRIPT                  "script"
#define DIR_YOLOV5                  "yolov5"
#define DIR_DATA                    "data"

// DATA =======================================================================
#include <QString>
struct MSG_RETURN {
    int code;
    QString msg;
};
// DATA =======================================================================

// FUNCTION ===================================================================
#include <QString>
#define TXT_INFO(msg)    ui->txtEditLog->append(QString("[INFO] %1").arg(msg))
#define TXT_WARN(msg)    ui->txtEditLog->append(QString("[WARN] %1").arg(msg))

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
#define SHOW_MSGBOX_INFO(msg) \
    QMessageBox::information(this, "Information", msg);
#define SHOW_MSGBOX_WARNING(msg) \
    QMessageBox::warning(this, "Warning", msg);
#define SHOW_MSGBOX_CRITICAL(msg) \
    QMessageBox::critical(this, "Critical", msg);

#include <QFileDialog>
// open folder
#define OPEN_FOLDER(edit) \
{ \
    QString folderPath = QFileDialog::getExistingDirectory(this, "Open Folder", QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks); \
    if (!folderPath.isEmpty()) { \
        edit->setText(folderPath); \
    } \
}

// open file
#define OPEN_FILE(edit) \
{ \
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", QString(), "All Files (*);;Text Files (*.txt);;Image Files (*.png *.jpg);;"); \
    if (!filePath.isEmpty()) { \
        edit->setText(filePath); \
    } \
}

#include <QPushButton>
// btn open folder
#define OPEN_FOLDER_BTN(btn, edit) \
connect(btn, &QPushButton::clicked, this, [this]() { OPEN_FOLDER(edit); })

// btn open file
#define OPEN_FILE_BTN(btn, edit) \
    connect(btn, &QPushButton::clicked, this, [this]() { OPEN_FILE(edit); })
// FUNCTION ===================================================================
#endif
