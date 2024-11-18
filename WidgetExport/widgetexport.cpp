#include "widgetexport.h"
#include "ui_widgetexport.h"

#include "../include/keydef.h"
#include "../util/mysetting.h"
#include "../util/mylog.h"

#include <QProcess>


WidgetExport::WidgetExport(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetExport)
{
    ui->setupUi(this);
    initExport();
}

WidgetExport::~WidgetExport()
{
    delete ui;
}

void WidgetExport::initExport() {
    OPEN_FILE_BTN(ui->tBtnPtModel, ui->lEditPtModel);
    OPEN_FOLDER_BTN(ui->tBtnDirOutput, ui->lEditDirOutput);
}

void WidgetExport::getCfgData() {
    m_data.list_type = MY_SETTING.getValue(CFG_GROUP_EXPORT, CFG_EXPORT_TYPE).split(',');
    m_data.pt_model = MY_SETTING.getValue(CFG_GROUP_EXPORT, CFG_EXPORT_PT_MODEL);
    m_data.dir_output = MY_SETTING.getValue(CFG_GROUP_EXPORT, CFG_EXPORT_DIR_OUTPUT);
}

QStringList getTypeList(QComboBox *box) {
    QStringList list = {};
    for(int i = 0; i < box->count(); ++i) {
        list.append(box->itemText(i));
    }
    return list;
}

void WidgetExport::save2Cfg() {
    MY_SETTING.setValue(CFG_GROUP_EXPORT, CFG_EXPORT_TYPE, m_data.list_type.join(','));
    MY_SETTING.setValue(CFG_GROUP_EXPORT, CFG_EXPORT_PT_MODEL, m_data.pt_model);
    MY_SETTING.setValue(CFG_GROUP_EXPORT, CFG_EXPORT_DIR_OUTPUT, m_data.dir_output);
    MY_LOG_INFO("config save: Group[{}]", CFG_GROUP_EXPORT);
    MY_LOG_INFO("{}: {}", CFG_EXPORT_TYPE, m_data.list_type);
    MY_LOG_INFO("{}: {}", CFG_EXPORT_PT_MODEL, m_data.pt_model);
    MY_LOG_INFO("{}: {}", CFG_EXPORT_DIR_OUTPUT, m_data.dir_output);
}

void WidgetExport::show2Ui() {
    ui->comboBoxType->clear();
    ui->comboBoxType->addItems(m_data.list_type);
    ui->lEditPtModel->setText(m_data.pt_model);
    ui->lEditDirOutput->setText(m_data.dir_output);
}

void WidgetExport::getUiData() {
    m_data.list_type = getTypeList(ui->comboBoxType);
    m_data.pt_model = ui->lEditPtModel->text();
    m_data.dir_output = ui->lEditDirOutput->text();
}


void WidgetExport::on_btnStartExport_clicked()
{
}


void WidgetExport::runScript(const QString& script_path, const QString& class_name, const QString& input_folder, const QString& output_folder)
{
    QProcess* process = new QProcess(this);

    connect(process, &QProcess::readyReadStandardOutput, this, &WidgetExport::onProcessOutput);
    connect(process, &QProcess::readyReadStandardError, this, &WidgetExport::onProcessError);
    connect(process, &QProcess::finished, this, &WidgetExport::onProcessFinished);

    for(int i = 0; i < m_data.list_type.size(); ++i) {
        QString pythonCommand = "python";
        QStringList arguments;
        arguments << "--weights" << m_data.pt_model
                  << "--project" << m_data.dir_output
                  << "--include" << m_data.list_type.at(i);

        process->start(pythonCommand, QStringList() << script_path << arguments);
    }

}


void WidgetExport::onProcessOutput()
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    if (process) {
        QByteArray output = process->readAllStandardOutput();
        TXT_INFO(QString::fromUtf8(output));
    }
}

void WidgetExport::onProcessError()
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    if (process) {
        QByteArray errorOutput = process->readAllStandardError();
        TXT_WARN(QString::fromUtf8(errorOutput));
    }
}

void WidgetExport::onProcessFinished(int exitCode)
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    if (process) {
        if (exitCode == QProcess::CrashExit) {
            TXT_WARN("Script crashed!");
        } else if (exitCode != 0) {
            TXT_WARN(QString("Script finished with error code: %1").arg(exitCode));
        } else {
            TXT_INFO("Script finished successfully!");
        }
        process->deleteLater();
    }
}
