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

void WidgetTrain::on_tBtnWeights_triggered(QAction *arg1)
{

}

void WidgetTrain::on_tBtnEditData_triggered(QAction *arg1)
{

}

void WidgetTrain::on_tBtnOutput_triggered(QAction *arg1)
{

}

#include <QDebug>

void WidgetTrain::on_btnStartTrain_clicked()
{
    qDebug() << m_data.weights;
    qDebug() << m_data.data;
    qDebug() << m_data.output;
}

void WidgetTrain::getCfgData() {

}

void WidgetTrain::getUiData() {
    m_data.weights = ui->lEditWeights->text();
    m_data.data = ui->lEditData->text();
    m_data.output = ui->lEditOutput->text();
    m_data.epoch = ui->sBoxEpoch->value();
    m_data.batch_size = ui->sBoxBatchSize->value();
    m_data.l_rate = ui->dsBoxLRate->value();
    m_data.img_size = ui->sBoxImgSize->value();
}

void WidgetTrain::show2Ui() {
    ui->lEditWeights->setText(m_data.weights);
    ui->lEditData->setText(m_data.data);
    ui->lEditOutput->setText(m_data.output);
    ui->sBoxEpoch->setValue(m_data.epoch);
    ui->sBoxBatchSize->setValue(m_data.batch_size);
    ui->dsBoxLRate->setValue(m_data.l_rate);
    ui->sBoxImgSize->setValue(m_data.img_size);
}

void WidgetTrain::save2Cfg() {

}
