#include "widgettrain.h"
#include "ui_widgettrain.h"

#include "../include/funcdef.h"
#include "../include/global.h"
#include "../include/keydef.h"

#include <QFileDialog>
#include <QProcess>
#include <QDateTime>

WidgetTrain::WidgetTrain(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetTrain)
{
    ui->setupUi(this);
    initTrain();
}

WidgetTrain::~WidgetTrain()
{
    delete ui;
}

void WidgetTrain::initTrain()
{
    getCfgData();
    show2Ui();
    OPEN_FILE_BTN(ui->tBtnWeights, ui->lEditWeights);
    OPEN_FILE_BTN(ui->tBtnCfg, ui->lEditCfg);
    OPEN_FILE_BTN(ui->tBtnHyp, ui->lEditHyp);
    OPEN_FILE_BTN(ui->tBtnData, ui->lEditData);
    OPEN_FOLDER_BTN(ui->tBtnProject, ui->lEditProject);
    m_process = new QProcess();
    connect(m_process, &QProcess::readyReadStandardOutput, this, &WidgetTrain::onProcessOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &WidgetTrain::onProcessError);
    connect(m_process, &QProcess::finished, this, &WidgetTrain::onProcessFinished);
}

void WidgetTrain::callTrain() {
    QStringList arguments {
        GLOBAL.SCRIPT_YOLO_TRAIN,
        "--weights", m_data.weights,
        "--cfg", m_data.cfg,
        "--hyp", m_data.hyp,
        "--data", m_data.data,
        "--project", m_data.project,
        "--name", m_data.name,
        "--epochs", QString::number(m_data.epochs),
        "--batch-size", QString::number(m_data.batch_size),
        "--imgsz", QString::number(m_data.img_size)
    };
    WIDGET_LOG_TRACE(QString("Script: %1").arg(arguments.join(' ')));
    runScript(arguments);
}

void WidgetTrain::on_btnStartTrain_clicked()
{
    getUiData();
    save2Cfg();
    LOG_INFO("config save: Group[{}]", CFG_GROUP_TRAIN);
    LOG_INFO("{}: {}", CFG_TRAIN_WEIGHTS, m_data.weights);
    LOG_INFO("{}: {}", CFG_TRAIN_CFG, m_data.cfg);
    LOG_INFO("{}: {}", CFG_TRAIN_HYP, m_data.hyp);
    LOG_INFO("{}: {}", CFG_TRAIN_DATA, m_data.data);
    LOG_INFO("{}: {}", CFG_TRAIN_PROJECT, m_data.project);
    LOG_INFO("{}: {}", CFG_TRAIN_NAME, m_data.name);
    LOG_INFO("{}: {}", CFG_TRAIN_EPOCHS, QString::number(m_data.epochs));
    LOG_INFO("{}: {}", CFG_TRAIN_BATCH_SIZE, QString::number(m_data.batch_size));
    LOG_INFO("{}: {}", CFG_TRAIN_IMG_SIZE, QString::number(m_data.img_size));
    callTrain();
}

void WidgetTrain::getCfgData()
{
    m_data.list_model = SETTING_GET(CFG_GROUP_TRAIN, CFG_TRAIN_MODEL).split(',');
    m_data.weights = SETTING_GET(CFG_GROUP_TRAIN, CFG_TRAIN_WEIGHTS);
    m_data.cfg = SETTING_GET(CFG_GROUP_TRAIN, CFG_TRAIN_CFG);
    m_data.hyp = SETTING_GET(CFG_GROUP_TRAIN, CFG_TRAIN_HYP);
    m_data.data = SETTING_GET(CFG_GROUP_TRAIN, CFG_TRAIN_DATA);
    m_data.project = SETTING_GET(CFG_GROUP_TRAIN, CFG_TRAIN_PROJECT);
    m_data.name = SETTING_GET(CFG_GROUP_TRAIN, CFG_TRAIN_NAME);
    m_data.epochs = SETTING_GET(CFG_GROUP_TRAIN, CFG_TRAIN_EPOCHS).toInt();
    m_data.batch_size = SETTING_GET(CFG_GROUP_TRAIN, CFG_TRAIN_BATCH_SIZE).toInt();
    m_data.img_size = SETTING_GET(CFG_GROUP_TRAIN, CFG_TRAIN_IMG_SIZE).toInt();
}

void WidgetTrain::getUiData() 
{
    m_data.weights = ui->lEditWeights->text();
    m_data.cfg = ui->lEditCfg->text();
    m_data.hyp = ui->lEditHyp->text();
    m_data.data = ui->lEditData->text();
    m_data.project = ui->lEditProject->text();
    m_data.name = ui->lEditName->text();
    m_data.epochs = ui->sBoxEpoch->value();
    m_data.batch_size = ui->sBoxBatchSize->value();
    m_data.img_size = ui->sBoxImgSize->value();
}

void WidgetTrain::show2Ui() 
{
    ui->lEditWeights->setText(m_data.weights);
    ui->lEditCfg->setText(m_data.cfg);
    ui->lEditHyp->setText(m_data.hyp);
    ui->lEditData->setText(m_data.data);
    ui->lEditProject->setText(m_data.project);
    ui->lEditName->setText(m_data.name);
    ui->sBoxEpoch->setValue(m_data.epochs);
    ui->sBoxBatchSize->setValue(m_data.batch_size);
    ui->sBoxImgSize->setValue(m_data.img_size);
}

void WidgetTrain::save2Cfg() 
{
    SETTING_SET(CFG_GROUP_TRAIN, CFG_TRAIN_WEIGHTS, m_data.weights);
    SETTING_SET(CFG_GROUP_TRAIN, CFG_TRAIN_CFG, m_data.cfg);
    SETTING_SET(CFG_GROUP_TRAIN, CFG_TRAIN_HYP, m_data.hyp);
    SETTING_SET(CFG_GROUP_TRAIN, CFG_TRAIN_DATA, m_data.data);
    SETTING_SET(CFG_GROUP_TRAIN, CFG_TRAIN_PROJECT, m_data.project);
    SETTING_SET(CFG_GROUP_TRAIN, CFG_TRAIN_NAME, m_data.name);
    SETTING_SET(CFG_GROUP_TRAIN, CFG_TRAIN_EPOCHS, QString::number(m_data.epochs));
    SETTING_SET(CFG_GROUP_TRAIN, CFG_TRAIN_BATCH_SIZE, QString::number(m_data.batch_size));
    SETTING_SET(CFG_GROUP_TRAIN, CFG_TRAIN_IMG_SIZE, QString::number(m_data.img_size));
}

void WidgetTrain::onProcessOutput()
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    if (process) {
        QByteArray output = process->readAllStandardOutput();
        WIDGET_LOG_INFO(QString::fromUtf8(output));
    }
}

void WidgetTrain::onProcessError()
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    if (process) {
        QByteArray errorOutput = process->readAllStandardError();
        WIDGET_LOG_WARN(QString::fromUtf8(errorOutput));
    }
}

void WidgetTrain::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    if (process) {
        if (exitStatus == QProcess::CrashExit) {
            WIDGET_LOG_WARN("Script crashed!");
        } else if (exitCode != 0) {
            WIDGET_LOG_WARN(QString("Script finished with error code: %1").arg(exitCode));
        } else {
            WIDGET_LOG_INFO("Script finished successfully!");
        }
        process->deleteLater();
    }
}

void WidgetTrain::runScript(const QStringList &arguments)
{
    m_process->start(GLOBAL.PYTHON, arguments);
    if (!m_process->waitForStarted()) {
        WIDGET_LOG_WARN("Failed to start script!");
    }
}

void WidgetTrain::stopScript() {
    if (m_process && m_process->state() == QProcess::Running) {
        m_process->terminate();
        if (!m_process->waitForFinished(3000)) {
            m_process->kill();
        }
        m_process->deleteLater();
        WIDGET_LOG_INFO("Script stopped.");
    }
    else {
        WIDGET_LOG_INFO("No Script is running.");
    }
}

void WidgetTrain::on_btnCancel_clicked()
{
    QMessageBox::StandardButton reply;
    reply = SHOW_MSGBOX_WARNING_REPLY(tr("are you sure to cancel?"), QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        stopScript();
    }
}

void WidgetTrain::on_tBtnName_clicked()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString timestamp = currentDateTime.toString("yyyyMMddHHmmss");
    ui->lEditName->setText(QString("%1").arg(timestamp));
}

