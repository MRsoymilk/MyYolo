#include "widgettrain.h"
#include "ui_widgettrain.h"

#include "../include/keydef.h"
#include "../util/mysetting.h"

#include <QFileDialog>

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

void WidgetTrain::on_btnStartTrain_clicked()
{
    MY_LOG_INFO("config save: Group[{}]", CFG_GROUP_TRAIN);
    MY_LOG_INFO("{}: {}", CFG_TRAIN_WEIGHTS, m_data.weights);
    MY_LOG_INFO("{}: {}", CFG_TRAIN_DATA, m_data.data);
    MY_LOG_INFO("{}: {}", CFG_TRAIN_OUTPUT, m_data.output);
    MY_LOG_INFO("{}: {}", CFG_TRAIN_EPOCH, QString::number(m_data.epoch));
    MY_LOG_INFO("{}: {}", CFG_TRAIN_BATCH_SIZE, QString::number(m_data.batch_size));
    MY_LOG_INFO("{}: {}", CFG_TRAIN_L_RATE, QString::number(m_data.l_rate));
    MY_LOG_INFO("{}: {}", CFG_TRAIN_IMG_SIZE, QString::number(m_data.img_size));
}

void WidgetTrain::getCfgData() 
{
    m_data.weights = MY_SETTING.getValue(CFG_GROUP_TRAIN, CFG_TRAIN_WEIGHTS);
    m_data.data = MY_SETTING.getValue(CFG_GROUP_TRAIN, CFG_TRAIN_DATA);
    m_data.output = MY_SETTING.getValue(CFG_GROUP_TRAIN, CFG_TRAIN_OUTPUT);
    m_data.epoch = MY_SETTING.getValue(CFG_GROUP_TRAIN, CFG_TRAIN_EPOCH).toInt();
    m_data.batch_size = MY_SETTING.getValue(CFG_GROUP_TRAIN, CFG_TRAIN_BATCH_SIZE).toInt();
    m_data.l_rate = MY_SETTING.getValue(CFG_GROUP_TRAIN, CFG_TRAIN_L_RATE).toDouble();
    m_data.img_size = MY_SETTING.getValue(CFG_GROUP_TRAIN, CFG_TRAIN_IMG_SIZE).toInt();
}

void WidgetTrain::getUiData() 
{
    m_data.weights = ui->lEditWeights->text();
    m_data.data = ui->lEditData->text();
    m_data.output = ui->lEditOutput->text();
    m_data.epoch = ui->sBoxEpoch->value();
    m_data.batch_size = ui->sBoxBatchSize->value();
    m_data.l_rate = ui->dsBoxLRate->value();
    m_data.img_size = ui->sBoxImgSize->value();
}

void WidgetTrain::show2Ui() 
{
    ui->lEditWeights->setText(m_data.weights);
    ui->lEditData->setText(m_data.data);
    ui->lEditOutput->setText(m_data.output);
    ui->sBoxEpoch->setValue(m_data.epoch);
    ui->sBoxBatchSize->setValue(m_data.batch_size);
    ui->dsBoxLRate->setValue(m_data.l_rate);
    ui->sBoxImgSize->setValue(m_data.img_size);
}

void WidgetTrain::save2Cfg() 
{
    MY_SETTING.setValue(CFG_GROUP_TRAIN, CFG_TRAIN_WEIGHTS, m_data.weights);
    MY_SETTING.setValue(CFG_GROUP_TRAIN, CFG_TRAIN_DATA, m_data.data);
    MY_SETTING.setValue(CFG_GROUP_TRAIN, CFG_TRAIN_OUTPUT, m_data.output);
    MY_SETTING.setValue(CFG_GROUP_TRAIN, CFG_TRAIN_EPOCH, QString::number(m_data.epoch));
    MY_SETTING.setValue(CFG_GROUP_TRAIN, CFG_TRAIN_BATCH_SIZE, QString::number(m_data.batch_size));
    MY_SETTING.setValue(CFG_GROUP_TRAIN, CFG_TRAIN_L_RATE, QString::number(m_data.l_rate));
    MY_SETTING.setValue(CFG_GROUP_TRAIN, CFG_TRAIN_IMG_SIZE, QString::number(m_data.img_size));
}
