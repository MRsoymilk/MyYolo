#include "widgetadddata.h"
#include "ui_widgetadddata.h"

#include "../include/keydef.h"
#include "../util/mysetting.h"

#include <QFileDialog>

WidgetAddData::WidgetAddData(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetAddData)
{
    ui->setupUi(this);
    getCfgData();
    show2Ui();
    connect(ui->tBtnInput, &QPushButton::clicked, this, [this](){ openFolder(ui->lEditDataInput); });
    connect(ui->tBtnOutput, &QPushButton::clicked, this, [this](){ openFolder(ui->lEditDataOutput); });
}

WidgetAddData::~WidgetAddData()
{
    delete ui;
}

void WidgetAddData::getCfgData()
{
    m_data.data_input = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_DATA_INPUT);
    m_data.data_output = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_DATA_OUTPUT);
    m_data.train = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_TRAIN).toInt();
    m_data.valid = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_VALID).toInt();
    m_data.test = MY_SETTING.getValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_TEST).toInt();
}

void WidgetAddData::getUiData()
{
    m_data.data_input = ui->lEditDataInput->text();
    m_data.data_output = ui->lEditDataOutput->text();
    m_data.train = ui->sBoxTrain->value();
    m_data.valid = ui->sBoxValid->value();
    m_data.test = ui->sBoxTest->value();
}

void WidgetAddData::show2Ui()
{
    ui->lEditDataInput->setText(m_data.data_input);
    ui->lEditDataInput->setText(m_data.data_output);
    ui->sBoxTrain->setValue(m_data.train);
    ui->sBoxValid->setValue(m_data.valid);
    ui->sBoxTest->setValue(m_data.test);
}

void WidgetAddData::save2Cfg()
{
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_DATA_INPUT, m_data.data_input);
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_DATA_OUTPUT, m_data.data_output);
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_TRAIN, QString::number(m_data.train));
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_VALID, QString::number(m_data.valid));
    MY_SETTING.setValue(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_TEST, QString::number(m_data.test));
}

void WidgetAddData::initAddData()
{

}

void WidgetAddData::on_tBtnInput_triggered(QAction *arg1)
{

}


void WidgetAddData::on_tBtnOutput_triggered(QAction *arg1)
{

}


void WidgetAddData::on_btnStartAddData_clicked()
{
    getUiData();
    TXT_INFO("data input: " + m_data.data_input);
    TXT_INFO("data output: " + m_data.data_output);
    TXT_INFO(QString("train: %1").arg(m_data.train));
    TXT_INFO(QString("train: %1").arg(m_data.valid));
    TXT_INFO(QString("train: %1").arg(m_data.test));

    // pre check
    if(m_data.data_input.isEmpty()) {
        SHOW_MSGBOX_CRITICAL("data input is empty!");
        return;
    }
    if(m_data.data_output.isEmpty()) {
        SHOW_MSGBOX_CRITICAL("data output is empty!");
        return;
    }
    if(m_data.train + m_data.valid + m_data.test != 10) {
        SHOW_MSGBOX_CRITICAL("train + valid + test must be 10!");
        return;
    }

    // process file
    QDir dir(m_data.data_input);
    QFileInfoList entries = dir.entryInfoList(QStringList() << "*.jpg", QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    int total = entries.count();
    int to_train = total * (m_data.train / 10);
    int to_valid = total * (m_data.valid / 10);
    int to_test = total - to_train - to_valid;
    foreach (const QFileInfo &entry, entries) {
        if(!entry.isFile()) {
            SHOW_MSGBOX_WARNING(QString("we dont handle: %1").arg(entry.absoluteFilePath()));
            continue;
        }
        qDebug() << "File path:" << entry.absoluteFilePath();
        TXT_INFO(QString("path %1").arg(entry.absoluteFilePath()));
        QString baseName = entry.baseName();

        // .xml and .txt
        QString xmlFilePath = dir.filePath(baseName + ".xml");
        QString txtFilePath = dir.filePath(baseName + ".txt");

        if(QFile::exists(xmlFilePath)) {
            TXT_INFO("exist xml " + xmlFilePath);
        }
        else {
            // mov jpg to dir todo
        }

        if(QFile::exists(txtFilePath)) {
            TXT_INFO("exist txt " + txtFilePath);
        }
        else {
            // generate txt

        }
    }
}

void WidgetAddData::openFolder(QLineEdit *edit) {
    // QString filePath = QFileDialog::getOpenFileName(this, "Open File", QString(),
    //                                                 "All Files (*);;Text Files (*.txt);;Image Files (*.png *.jpg);;");
    // if (!filePath.isEmpty()) {
    //     edit->setText(filePath);
    // }

    QString folderPath = QFileDialog::getExistingDirectory(this, "select dir", QString(),
                                                           QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!folderPath.isEmpty()) {
        edit->setText(folderPath);
    }
}

void WidgetAddData::openFile(QLineEdit *edit)
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", QString(),
                                                    "All Files (*);;Text Files (*.txt);;Image Files (*.png *.jpg);;");
    if (!filePath.isEmpty()) {
        edit->setText(filePath);
    }
}

