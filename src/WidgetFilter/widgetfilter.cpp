#include "widgetfilter.h"
#include "ui_widgetfilter.h"

#include "../include/funcdef.h"
#include "../include/global.h"
#include "../include/keydef.h"
#include "../util/mylog.h"
#include "../util/mysetting.h"

WidgetFilter::WidgetFilter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetFilter)
{
    ui->setupUi(this);
    initFilter();
}

WidgetFilter::~WidgetFilter()
{
    delete ui;
}

void WidgetFilter::initFilter()
{
    getCfgData();
    show2Ui();
    OPEN_FOLDER_BTN(ui->tBtnDirInput, ui->lEditDirInput);
    OPEN_FOLDER_BTN(ui->tBtnDirOutput, ui->lEditDirOutput);
    m_process = new QProcess();
    connect(m_process, &QProcess::readyReadStandardOutput, this, &WidgetFilter::onProcessOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &WidgetFilter::onProcessError);
    connect(m_process, &QProcess::finished, this, &WidgetFilter::onProcessFinished);
}

void WidgetFilter::getCfgData()
{
    m_data.dir_input = MY_SETTING.getValue(CFG_GROUP_FILTER, CFG_FILTER_DIR_INPUT);
    m_data.dir_output = MY_SETTING.getValue(CFG_GROUP_FILTER, CFG_FILTER_DIR_OUTPUT);
    m_data.is_ssim = MY_SETTING.getValue(CFG_GROUP_FILTER, CFG_FILTER_SSIM).toInt();
    m_data.threshold_ssim = MY_SETTING.getValue(CFG_GROUP_FILTER, CFG_FILTER_THRESHOLD_SSIM).toDouble();
    m_data.batch_ssim = MY_SETTING.getValue(CFG_GROUP_FILTER, CFG_FILTER_BATCH_SSIM).toInt();
}

void WidgetFilter::save2Cfg()
{
    MY_SETTING.setValue(CFG_GROUP_FILTER, CFG_FILTER_DIR_INPUT, m_data.dir_input);
    MY_SETTING.setValue(CFG_GROUP_FILTER, CFG_FILTER_DIR_OUTPUT, m_data.dir_output);
    MY_SETTING.setValue(CFG_GROUP_FILTER, CFG_FILTER_SSIM, QString::number(m_data.is_ssim));
    MY_SETTING.setValue(CFG_GROUP_FILTER, CFG_FILTER_THRESHOLD_SSIM, QString::number(m_data.threshold_ssim));
    MY_SETTING.setValue(CFG_GROUP_FILTER, CFG_FILTER_BATCH_SSIM, QString::number(m_data.batch_ssim));
}

void WidgetFilter::show2Ui()
{
    ui->lEditDirInput->setText(m_data.dir_input);
    ui->lEditDirOutput->setText(m_data.dir_output);
    if (m_data.is_ssim) {
        ui->checkBoxSSIM->setCheckState(Qt::Checked);
    } else {
        ui->checkBoxSSIM->setCheckState(Qt::Unchecked);
    }
    ui->doubleSpinBoxThresholdSSIM->setValue(m_data.threshold_ssim);
    ui->sBoxBatchSize->setValue(m_data.batch_ssim);
}

void WidgetFilter::getUiData()
{
    m_data.dir_input = ui->lEditDirInput->text();
    m_data.dir_output = ui->lEditDirOutput->text();
    if (ui->checkBoxSSIM->isChecked()) {
        m_data.is_ssim = 1;
    } else {
        m_data.is_ssim = 0;
    }
    m_data.threshold_ssim = ui->doubleSpinBoxThresholdSSIM->value();
    m_data.batch_ssim = ui->sBoxBatchSize->value();
}

void WidgetFilter::runScript(const QStringList &arguments)
{
    m_process->startDetached(GLOBAL.PYTHON, arguments);
}

void WidgetFilter::callMoveSimilarImgs()
{
    QStringList arguments_move_similar{
        GLOBAL.SCRIPT_MOVE_SIMILAR_IMG,
        "--input_folder", m_data.dir_input,
        "--output_folder", m_data.dir_output,
        "--similarity_threshold", QString::number(m_data.threshold_ssim),
        "--batch_size", QString::number(m_data.batch_ssim)
    };
    TXT_INFO(QString("Script: %1").arg(arguments_move_similar.join(' ')));
    runScript(arguments_move_similar);
}

void WidgetFilter::on_btnStartFilter_clicked()
{
    getUiData();
    save2Cfg();
    if(m_data.dir_input.isEmpty()) {
        SHOW_MSGBOX_CRITICAL("dir input cannot be empty!");
        return;
    }
    if(m_data.dir_output.isEmpty()) {
        SHOW_MSGBOX_CRITICAL("dir output cannot be empty!");
        return;
    }
    if (m_data.is_ssim) {
        callMoveSimilarImgs();
    }
}

void WidgetFilter::onProcessOutput()
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    if (process) {
        QByteArray output = process->readAllStandardOutput();
        TXT_INFO(QString::fromUtf8(output));
    }
}

void WidgetFilter::onProcessError()
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    if (process) {
        QByteArray errorOutput = process->readAllStandardError();
        TXT_WARN(QString::fromUtf8(errorOutput));
    }
}

void WidgetFilter::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
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
