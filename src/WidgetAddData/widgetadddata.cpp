#include "widgetadddata.h"
#include "ui_widgetadddata.h"

#include "../include/funcdef.h"
#include "../include/keydef.h"
#include "../util/mylog.h"
#include "../util/mysetting.h"
#include "../include/global.h"
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
        if(classList.at(row).isEmpty()) {
            continue;
        }
        widget->setItem(row, 0, new QTableWidgetItem(QString::number(row)));
        widget->setItem(row, 1, new QTableWidgetItem(classList.at(row)));
    }
}

void WidgetAddData::getCfgData() {
    m_data.dir_img = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_DIR_INPUT);
    QStringList tags = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_LIST_TAG).split(',');
    tags.removeAll("");
    m_data.list_tag = tags;
    m_data.train = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_TRAIN).toDouble();
    m_data.valid = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_VALID).toDouble();
    m_data.test = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_TEST).toDouble();
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
    m_data.dir_img = ui->lEditDirInput->text();
    m_data.list_tag = getClassList(ui->tableWidgetTag);
    m_data.train = ui->doubleSpinBoxTrain->value();
    m_data.valid = ui->doubleSpinBoxValid->value();
    m_data.test = ui->doubleSpinBoxTest->value();
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
    ui->lEditDirInput->setText(m_data.dir_img);
    setClassList(ui->tableWidgetTag, m_data.list_tag);
    ui->doubleSpinBoxTrain->setValue(m_data.train);
    ui->doubleSpinBoxValid->setValue(m_data.valid);
    ui->doubleSpinBoxTest->setValue(m_data.test);
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
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_DIR_INPUT, m_data.dir_img);
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
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_DIR_INPUT, m_data.dir_img);
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_DIR_OUTPUT_DIVIDE, m_data.dir_output_divide);
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_TRAIN, m_data.train);
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_VALID, m_data.valid);
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_TEST, m_data.test);
    MY_LOG_INFO("{}: {}", CFG_ADD_DATA_DIR_INPUT, m_data.dir_img);
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
    m_process = new QProcess();
    connect(m_process, &QProcess::readyReadStandardOutput, this, &WidgetAddData::onProcessOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &WidgetAddData::onProcessError);
    connect(m_process, &QProcess::finished, this, &WidgetAddData::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred, [](QProcess::ProcessError error) {
        qDebug() << "Process error occurred: " << error;
    });
}

void WidgetAddData::callXml2Txt() {
    // python xml2txt.py --class_names car,person --input_folder /path/to/input_folder --output_folder /path/to/output_folder
    QStringList arguments_xml2txt {
        GLOBAL.SCRIPT_XML2TXT,
        "--class_name", m_data.list_tag.join(','),
        "--input_folder", m_data.dir_img,
        "--output_folder", m_data.dir_img
    };
    TXT_INFO(QString("Script: %1").arg(arguments_xml2txt.join(' ')));
    runScript(arguments_xml2txt);
}

void WidgetAddData::callNoXml2Txt() {
    // python no_xml2txt.py --input_folder /path/to/input_folder --output_folder /path/to/output_folder
    QStringList arguments_no_xml2txt {
        GLOBAL.SCRIPT_NO_XML2TXT,
        "--input_folder", m_data.dir_img,
        "--output_folder", m_data.dir_img
    };
    TXT_INFO(QString("Script: %1").arg(arguments_no_xml2txt.join(' ')));
    runScript(arguments_no_xml2txt);
}

void WidgetAddData::callRename() {
    QStringList arguments_rename{GLOBAL.SCRIPT_RENAME, "--input_folder", m_data.dir_img};
    if (!m_data.rename_prefix.isEmpty()) {
        arguments_rename << "--prefix" << m_data.rename_prefix;
    }
    if (!m_data.rename_suffix.isEmpty()) {
        arguments_rename << "--suffix" << m_data.rename_suffix;
    }
    TXT_INFO(QString("Script: %1").arg(arguments_rename.join(' ')));
    runScript(arguments_rename);
}

void WidgetAddData::callReNoTag() {
    QStringList arguments_re_notag{GLOBAL.SCRIPT_RE_NOTAG, "--input_folder", m_data.dir_img};
    if (!m_data.re_notag_prefix.isEmpty()) {
        arguments_re_notag << "--prefix" << m_data.re_notag_prefix;
    }
    if (!m_data.re_notag_suffix.isEmpty()) {
        arguments_re_notag << "--suffix" << m_data.re_notag_suffix;
    }
    TXT_INFO(QString("Script: %1").arg(arguments_re_notag.join(' ')));
    runScript(arguments_re_notag);
}

void WidgetAddData::callSpliteDataset() {
    QStringList arguments_splitedata {
        GLOBAL.SCRIPT_SPLIT_DATASET,
        "--dir_in", m_data.dir_img,
        "--dir_out", m_data.dir_output_divide,
        "--train-ratio", QString::number(m_data.train),
        "--val-ratio", QString::number(m_data.valid),
        "--test-ratio", QString::number(m_data.test),
        "--shuffle", QString::number(m_data.is_shuffle)
    };
    TXT_INFO(QString("Script: %1").arg(arguments_splitedata.join(' ')));
    runScript(arguments_splitedata);
}

void WidgetAddData::on_btnStartAddData_clicked() {
    getUiData();
    save2Cfg();
    TXT_INFO("dir input: " + m_data.dir_img);
    // TXT_INFO("scirpt: " + m_data.script);
    TXT_INFO("dir output: " + m_data.dir_output_divide);
    TXT_INFO(QString("train: %1").arg(m_data.train));
    TXT_INFO(QString("valid: %1").arg(m_data.valid));
    TXT_INFO(QString("test: %1").arg(m_data.test));

    // pre check
    if(m_data.dir_img.isEmpty()) {
        SHOW_MSGBOX_CRITICAL("data input can not be empty!");
        return;
    }
    if(m_data.dir_output_divide.isEmpty()) {
        SHOW_MSGBOX_CRITICAL("data output divide can not be empty!");
        return;
    }
    const double epsilon = 1e-6;
    double val = qAbs((m_data.train + m_data.valid + m_data.test) - 1.0);
    if (val > epsilon) {
        SHOW_MSGBOX_CRITICAL("train + valid + test must be 1.0!");
        return;
    }

    // generate txt file: call xml2txt.py
    callXml2Txt();
    // generate no txt file: call no_xml2txt.py
    callNoXml2Txt();
    // reaname: call rename.py
    if(ui->checkBoxRename->isChecked()) {
        if(ui->lEditRenamePrefix->text().isEmpty() && ui->lEditRenameSuffix->text().isEmpty()) {
            TXT_WARN("empty prefix and suffix");
        }
        else {
            callRename();
        }
    }
    // rename: call re_notag.py
    if(ui->checkBoxReNoTag->isChecked()) {
        if(ui->lEditReNoTagPrefix->text().isEmpty() && ui->lEditReNoTagSuffix->text().isEmpty()) {
            TXT_WARN("empty prefix and suffix");
        }
        else {
            callReNoTag();
        }
    }
    // add train.txt, test.txt, val.txt
    callSpliteDataset();
}

void WidgetAddData::runScript(const QStringList &arguments)
{
    m_process->startDetached(GLOBAL.PYTHON, arguments);
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

void WidgetAddData::on_btnAddTag_clicked()
{
    int newRow = ui->tableWidgetTag->rowCount();
    ui->tableWidgetTag->insertRow(newRow);

    QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(newRow));
    ui->tableWidgetTag->setItem(newRow, 0, idItem);

    QTableWidgetItem *classNameItem = new QTableWidgetItem();
    ui->tableWidgetTag->setItem(newRow, 1, classNameItem);

    QPushButton *deleteButton = new QPushButton("Delete");
    ui->tableWidgetTag->setCellWidget(newRow, 2, deleteButton);

    connect(deleteButton, &QPushButton::clicked, this, [this, deleteButton]() {
        int row = ui->tableWidgetTag->indexAt(deleteButton->pos()).row();
        ui->tableWidgetTag->removeRow(row);
    });
}

void WidgetAddData::on_btnRemoveTag_clicked()
{
    int row = ui->tableWidgetTag->currentRow();
    if (row >= 0) {
        ui->tableWidgetTag->removeRow(row);
    }
}

void WidgetAddData::on_tableWidgetTag_cellDoubleClicked(int row, int column)
{
    ui->tableWidgetTag->editItem(ui->tableWidgetTag->item(row, column));
}

