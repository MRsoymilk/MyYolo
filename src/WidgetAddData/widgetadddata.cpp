#include "widgetadddata.h"
#include "ui_widgetadddata.h"

#include "../include/funcdef.h"
#include "../include/keydef.h"
#include "../util/mylog.h"
#include "../util/mysetting.h"

#include <QFileDialog>
#include <QProcess>
#include <qcombobox.h>

WidgetAddData::WidgetAddData(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetAddData)
{
    ui->setupUi(this);
    initAddData();
}

WidgetAddData::~WidgetAddData() {
    delete ui;
}

QStringList getClassList(QTableWidget *widget) {
    QStringList list{};
    int rowCount = widget->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        QString cellValue = widget->item(i, 1)->text();
        list.append(cellValue);
    }
    return list;
}

void setClassList(QTableWidget *widget, const QStringList &classList) {
    widget->setRowCount(classList.size());
    for (int row = 0; row < classList.size(); ++row) {
        widget->setItem(row, 0, new QTableWidgetItem(QString::number(row)));
        widget->setItem(row, 1, new QTableWidgetItem(classList.at(row)));
    }
}

void WidgetAddData::getCfgData() {
    m_data.dir_input = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_DIR_INPUT);
    m_data.list_tag = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_LIST_TAG).split(',');
    m_data.train = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_TRAIN).toInt();
    m_data.valid = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_VALID).toInt();
    m_data.test = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_TEST).toInt();
    m_data.dir_output_divide = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_DIR_OUTPUT_DIVIDE);
    m_data.is_shuffle = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_IS_SHUFFLE).toInt();
    m_data.is_rename = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_IS_RENAME).toInt();
    m_data.rename_prefix = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RENAME_PREFIX);
    m_data.rename_suffix = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RENAME_SUFFIX);
    m_data.is_rename_no_tag = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_IS_RENAME_NOTAG).toInt();
    m_data.re_notag_prefix = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RE_NOTAG_PREFIX);
    m_data.re_notag_suffix = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RE_NOTAG_SUFFIX);
}

void WidgetAddData::getUiData() {
    m_data.dir_input = ui->lEditDirInput->text();
    m_data.list_tag = getClassList(ui->tableWidgetTag);
    m_data.train = ui->sBoxTrain->value();
    m_data.valid = ui->sBoxValid->value();
    m_data.test = ui->sBoxTest->value();
    m_data.dir_output_divide = ui->lEditDirOutputDivide->text();
    m_data.is_shuffle = ui->checkBoxShuffle->isChecked();
    m_data.is_rename = ui->checkBoxRename->isChecked();
    m_data.rename_prefix = ui->lEditRenamePrefix->text();
    m_data.rename_suffix = ui->lEditRenameSuffix->text();
    m_data.is_rename_no_tag = ui->checkBoxReNoTag->isChecked();
    m_data.re_notag_prefix = ui->lEditReNoTagPrefix->text();
    m_data.re_notag_suffix = ui->lEditReNoTagSuffix->text();
}

void WidgetAddData::show2Ui() {
    ui->lEditDirInput->setText(m_data.dir_input);
    setClassList(ui->tableWidgetTag, m_data.list_tag);
    ui->sBoxTrain->setValue(m_data.train);
    ui->sBoxValid->setValue(m_data.valid);
    ui->sBoxTest->setValue(m_data.test);
    ui->lEditDirOutputDivide->setText(m_data.dir_output_divide);
    ui->checkBoxShuffle->setCheckState(m_data.is_shuffle ? Qt::Checked : Qt::Unchecked);
    ui->checkBoxRename->setCheckState(m_data.is_rename ? Qt::Checked : Qt::Unchecked);
    ui->lEditRenamePrefix->setText(m_data.rename_prefix);
    ui->lEditRenameSuffix->setText(m_data.rename_suffix);
    ui->checkBoxReNoTag->setCheckState(m_data.is_rename_no_tag ? Qt::Checked : Qt::Unchecked);
    ui->lEditReNoTagPrefix->setText(m_data.re_notag_prefix);
    ui->lEditReNoTagSuffix->setText(m_data.re_notag_suffix);
}

void WidgetAddData::save2Cfg() {
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_DIR_INPUT, m_data.dir_input);
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_LIST_TAG, m_data.list_tag.join(','));
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_TRAIN, QString::number(m_data.train));
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_VALID, QString::number(m_data.valid));
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_TEST, QString::number(m_data.test));
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_DIR_OUTPUT_DIVIDE, m_data.dir_output_divide);
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_IS_SHUFFLE, QString::number(m_data.is_shuffle));
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_IS_RENAME, QString::number(m_data.is_rename));
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RENAME_PREFIX, m_data.rename_prefix);
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RENAME_SUFFIX, m_data.rename_suffix);
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_IS_RENAME_NOTAG, QString::number(m_data.is_rename_no_tag));
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RE_NOTAG_PREFIX, m_data.re_notag_prefix);
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RE_NOTAG_SUFFIX, m_data.re_notag_suffix);

    MY_LOG_INFO("config save: Group[{}]", CFG_GROUP_ADD_DATA);
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_DIR_INPUT, m_data.dir_input);
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_DIR_OUTPUT_DIVIDE, m_data.dir_output_divide);
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_TRAIN, m_data.train);
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_VALID, m_data.valid);
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_TEST, m_data.test);
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_DIR_INPUT, m_data.dir_input);
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_DIR_OUTPUT_DIVIDE, m_data.dir_output_divide);
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_TRAIN, m_data.train);
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_VALID, m_data.valid);
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_TEST, m_data.test);
}

void WidgetAddData::initAddData() {
    getCfgData();
    show2Ui();
    OPEN_FOLDER_BTN(ui->tBtnDirInput, ui->lEditDirInput);
    OPEN_FOLDER_BTN(ui->tBtnDirOutputDivide, ui->lEditDirOutputDivide);
}


void WidgetAddData::on_btnStartAddData_clicked() {
    getUiData();
    save2Cfg();
    TXT_INFO("dir input: " + m_data.dir_input);
    // TXT_INFO("scirpt: " + m_data.script);
    TXT_INFO("dir output: " + m_data.dir_output_divide);
    TXT_INFO(QString("train: %1").arg(m_data.train));
    TXT_INFO(QString("valid: %1").arg(m_data.valid));
    TXT_INFO(QString("test: %1").arg(m_data.test));

    // pre check
    if(m_data.dir_input.isEmpty()) {
        SHOW_MSGBOX_CRITICAL("data input can not be empty!");
        return;
    }
    if(m_data.dir_output_divide.isEmpty()) {
        SHOW_MSGBOX_CRITICAL("data output divide can not be empty!");
        return;
    }
    if(m_data.train + m_data.valid + m_data.test != 10) {
        SHOW_MSGBOX_CRITICAL("train + valid + test must be 10!");
        return;
    }

    // process file
    QDir dir(m_data.dir_input);
    QFileInfoList entries = dir.entryInfoList(QStringList() << "*.jpg", QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    int total = entries.count();
    int to_train = total * (m_data.train / 10.0);
    int to_valid = total * ((m_data.train + m_data.valid) / 10.0);
    int to_test = total - to_train - to_valid;
    TXT_INFO(QString("count of train: %1").arg(to_train));
    TXT_INFO(QString("count of valid: %1").arg(to_valid));
    TXT_INFO(QString("count of test: %1").arg(to_test));
    // runScript(m_data.script, m_data.tag, m_data.dir_input, m_data.dir_output);
    QStringList listTrain{}, listValid{}, listTest{};
    int count = 1;
    foreach (const QFileInfo &entry, entries) {
        if(!entry.isFile()) {
            SHOW_MSGBOX_WARNING(QString("we dont handle: %1").arg(entry.absoluteFilePath()));
            continue;
        }
        TXT_INFO(QString("path %1").arg(entry.absoluteFilePath()));
        QString baseName = entry.baseName();
        // .xml and .txt
        QString xmlFilePath = dir.filePath(baseName + ".xml");
        QString txtFilePath = dir.filePath(baseName + ".txt");

        if(QFile::exists(xmlFilePath) && QFile::exists(txtFilePath)) {
            TXT_INFO(QString("exist xml %1, txt %2").arg(xmlFilePath, txtFilePath));
            if(count <= to_train) {
                listTrain.append(entry.absoluteFilePath());
            }
            else if(count > to_train && count <= to_valid) {
                listTest.append(entry.absoluteFilePath());
            }
            else {
                listValid.append(entry.absoluteFilePath());
            }
        }
        else {
            // mov jpg to dir todo
            // QFile::rename(entry.absoluteFilePath(), m_data.dir_todo + "/" + entry.fileName());
        }
        ++count;
    }
}

void WidgetAddData::runScript(const QString& script_path, const QString& class_name, const QString& input_folder, const QString& output_folder)
{
    QProcess* process = new QProcess(this);
    QString pythonCommand = "python";
    QStringList arguments;
    arguments << "--class_name" << class_name
              << "--input_folder" << input_folder
              << "--output_folder" << output_folder;

    connect(process, &QProcess::readyReadStandardOutput, this, &WidgetAddData::onProcessOutput);
    connect(process, &QProcess::readyReadStandardError, this, &WidgetAddData::onProcessError);
    connect(process, &QProcess::finished, this, &WidgetAddData::onProcessFinished);

    process->start(pythonCommand, QStringList() << script_path << arguments);
}

void WidgetAddData::onProcessOutput()
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    if (process) {
        QByteArray output = process->readAllStandardOutput();
        TXT_INFO(QString::fromUtf8(output));
    }
}

void WidgetAddData::onProcessError()
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    if (process) {
        QByteArray errorOutput = process->readAllStandardError();
        TXT_WARN(QString::fromUtf8(errorOutput));
    }
}

void WidgetAddData::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    if (process) {
        if (exitStatus == QProcess::CrashExit) {
            TXT_WARN("Script crashed!");
        } else if (exitCode != 0) {
            TXT_WARN(QString("Script finished with error code: %1").arg(exitCode));
        } else {
            TXT_INFO("Script finished successfully!");
        }
        process->deleteLater();
    }
}

void WidgetAddData::on_btnHandleNoGap_clicked()
{
    getUiData();
    QDir dir("");

    QFileInfoList entries = dir.entryInfoList(QStringList() << "*.jpg", QDir::Files | QDir::NoDotAndDotDot);

    foreach (const QFileInfo &entry, entries) {
        QString baseName = entry.baseName();

        QString xmlFilePath = dir.filePath(baseName + ".xml");

        if (QFile::exists(entry.absoluteFilePath()) && QFile::exists(xmlFilePath)) {
            QString newJpgFilePath = dir.filePath(baseName + "_no_gap.jpg");
            QString newXmlFilePath = dir.filePath(baseName + "_no_gap.xml");

            if (QFile::rename(entry.absoluteFilePath(), newJpgFilePath)) {
                TXT_INFO(QString("Renamed jpg to: %1").arg(newJpgFilePath));
            }
            else {
                TXT_WARN(QString("Failed to rename jpg: %1").arg(entry.absoluteFilePath()));
            }

            if (QFile::rename(xmlFilePath, newXmlFilePath)) {
                TXT_INFO(QString("Renamed xml to: %1").arg(newXmlFilePath));
            }
            else {
                TXT_WARN(QString("Failed to rename xml: %1").arg(xmlFilePath));
            }

            QString txtFilePath = dir.filePath(baseName + "_no_gap.txt");
            QFile txtFile(txtFilePath);
            if (txtFile.open(QIODevice::WriteOnly)) {
                txtFile.close();
                TXT_INFO(QString("Created empty txt file: %1").arg(txtFilePath));
            }
            else {
                TXT_WARN(QString("Failed to create txt file: %1").arg(txtFilePath));
            }
        }
        else {
            TXT_WARN(QString("Missing jpg or xml for %1").arg(entry.absoluteFilePath()));
        }
    }
}
