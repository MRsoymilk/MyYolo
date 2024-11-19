#include "widgettrain.h"
#include "ui_widgettrain.h"

#include "../include/keydef.h"
#include "../util/mysetting.h"
#include "../util/mylog.h"

#include <QFileDialog>
#include <QProcess>

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
    OPEN_FILE_BTN(ui->tBtnWeights, ui->lEditWeights);
    OPEN_FILE_BTN(ui->tBtnData, ui->lEditData);
    OPEN_FOLDER_BTN(ui->tBtnOutput, ui->lEditOutput);
}

void WidgetTrain::runScript(const QString &base_dir, const QString& script) {
    QProcess *process = new QProcess(this);
    QString pythonCommand = "python";
    QStringList arguments;
    arguments << "--weights" << m_data.weights
              << "--data" << m_data.data
              << "--batch_size" << QString::number(m_data.batch_size)
              << "--hyp" << "hyp.yaml"
              << "--imgsz" << QString::number(m_data.img_size)
              << "--epochs" << QString::number(m_data.epoch);
    process->start(pythonCommand, QStringList() << script << arguments);
}

void WidgetTrain::on_btnStartTrain_clicked()
{
    MY_LOG_INFO("config save: Group[{}]", CFG_GROUP_TRAIN);
    MY_LOG_INFO("{}: {}", CFG_TRAIN_WEIGHTS, m_data.weights);
    MY_LOG_INFO("{}: {}", CFG_TRAIN_DATA, m_data.data);
    MY_LOG_INFO("{}: {}", CFG_TRAIN_OUTPUT, m_data.output);
    MY_LOG_INFO("{}: {}", CFG_TRAIN_EPOCH, QString::number(m_data.epoch));
    MY_LOG_INFO("{}: {}", CFG_TRAIN_BATCH_SIZE, QString::number(m_data.batch_size));
    MY_LOG_INFO("{}: {}", CFG_TRAIN_IMG_SIZE, QString::number(m_data.img_size));
}

void WidgetTrain::getCfgData() 
{
    m_data.list_model = MY_SETTING.getValue(CFG_GROUP_TRAIN, CFG_TRAIN_MODEL).split(',');
    m_data.weights = MY_SETTING.getValue(CFG_GROUP_TRAIN, CFG_TRAIN_WEIGHTS);
    m_data.data = MY_SETTING.getValue(CFG_GROUP_TRAIN, CFG_TRAIN_DATA);
    m_data.output = MY_SETTING.getValue(CFG_GROUP_TRAIN, CFG_TRAIN_OUTPUT);
    m_data.epoch = MY_SETTING.getValue(CFG_GROUP_TRAIN, CFG_TRAIN_EPOCH).toInt();
    m_data.batch_size = MY_SETTING.getValue(CFG_GROUP_TRAIN, CFG_TRAIN_BATCH_SIZE).toInt();
    m_data.img_size = MY_SETTING.getValue(CFG_GROUP_TRAIN, CFG_TRAIN_IMG_SIZE).toInt();
}

void WidgetTrain::getUiData() 
{
    m_data.weights = ui->lEditWeights->text();
    m_data.data = ui->lEditData->text();
    m_data.output = ui->lEditOutput->text();
    m_data.epoch = ui->sBoxEpoch->value();
    m_data.batch_size = ui->sBoxBatchSize->value();
    m_data.img_size = ui->sBoxImgSize->value();
}

void WidgetTrain::show2Ui() 
{
    ui->lEditWeights->setText(m_data.weights);
    ui->lEditData->setText(m_data.data);
    ui->lEditOutput->setText(m_data.output);
    ui->sBoxEpoch->setValue(m_data.epoch);
    ui->sBoxBatchSize->setValue(m_data.batch_size);
    ui->sBoxImgSize->setValue(m_data.img_size);
}

void WidgetTrain::save2Cfg() 
{
    MY_SETTING.setValue(CFG_GROUP_TRAIN, CFG_TRAIN_WEIGHTS, m_data.weights);
    MY_SETTING.setValue(CFG_GROUP_TRAIN, CFG_TRAIN_DATA, m_data.data);
    MY_SETTING.setValue(CFG_GROUP_TRAIN, CFG_TRAIN_OUTPUT, m_data.output);
    MY_SETTING.setValue(CFG_GROUP_TRAIN, CFG_TRAIN_EPOCH, QString::number(m_data.epoch));
    MY_SETTING.setValue(CFG_GROUP_TRAIN, CFG_TRAIN_BATCH_SIZE, QString::number(m_data.batch_size));
    MY_SETTING.setValue(CFG_GROUP_TRAIN, CFG_TRAIN_IMG_SIZE, QString::number(m_data.img_size));
}
