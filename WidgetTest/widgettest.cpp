#include "widgettest.h"
#include "ui_widgettest.h"

#include <QFileDialog>
#include <QDir>

#include "../include/keydef.h"
#include "../util/mylog.h"
#include "../util/mysetting.h"


WidgetTest::WidgetTest(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetTest)
{
    ui->setupUi(this);
    initTest();
}

WidgetTest::~WidgetTest()
{
    delete ui;
}

void WidgetTest::initTest() {
    getCfgData();
    show2Ui();
    OPEN_FILE_BTN(ui->tBtnModel, ui->lEditModel);
    OPEN_FOLDER_BTN(ui->tBtnDirInput, ui->lEditDirInput);
    OPEN_FOLDER_BTN(ui->tBtnDirOutput, ui->lEditDirOutput);
}

void WidgetTest::getUiData() {
    m_data.model_height = ui->sBoxModelHeight->value();
    m_data.model_width = ui->sBoxModelWidth->value();
    m_data.model = ui->lEditModel->text();
    m_data.dir_input = ui->lEditDirInput->text();
    m_data.dir_output = ui->lEditDirOutput->text();
}

void WidgetTest::getCfgData() {
    m_data.model_height = MY_SETTING.getValue(CFG_GROUP_TEST, CFG_TEST_MODEL_HEIGHT).toInt();
    m_data.model_width = MY_SETTING.getValue(CFG_GROUP_TEST, CFG_TEST_MODEL_WIDTH).toInt();
    m_data.model = MY_SETTING.getValue(CFG_GROUP_TEST, CFG_TEST_MODEL);
    m_data.dir_input = MY_SETTING.getValue(CFG_GROUP_TEST, CFG_TEST_DIR_INPUT);
    m_data.dir_output = MY_SETTING.getValue(CFG_GROUP_TEST, CFG_TEST_DIR_OUTPUT);
}

void WidgetTest::save2Cfg() {
    MY_SETTING.setValue(CFG_GROUP_TEST, CFG_TEST_MODEL_HEIGHT, QString::number(m_data.model_height));
    MY_SETTING.setValue(CFG_GROUP_TEST, CFG_TEST_MODEL_WIDTH, QString::number(m_data.model_width));
    MY_SETTING.setValue(CFG_GROUP_TEST, CFG_TEST_MODEL, m_data.model);
    MY_SETTING.setValue(CFG_GROUP_TEST, CFG_TEST_DIR_INPUT, m_data.dir_input);
    MY_SETTING.setValue(CFG_GROUP_TEST, CFG_TEST_DIR_OUTPUT, m_data.dir_output);
    MY_LOG_INFO("config save: Group[{}]", CFG_GROUP_TEST);
    MY_LOG_INFO("{}: {}", CFG_TEST_MODEL_HEIGHT, m_data.model_height);
    MY_LOG_INFO("{}: {}", CFG_TEST_MODEL_WIDTH, m_data.model_width);
    MY_LOG_INFO("{}: {}", CFG_TEST_MODEL, m_data.model);
    MY_LOG_INFO("{}: {}", CFG_TEST_DIR_INPUT, m_data.dir_input);
    MY_LOG_INFO("{}: {}", CFG_TEST_DIR_OUTPUT, m_data.dir_output);
}

void WidgetTest::show2Ui() {
    ui->sBoxModelHeight->setValue(m_data.model_height);
    ui->sBoxModelWidth->setValue(m_data.model_width);
    ui->lEditModel->setText(m_data.model);
    ui->lEditDirInput->setText(m_data.dir_input);
    ui->lEditDirOutput->setText(m_data.dir_output);
}

void WidgetTest::testOnnx() {
    if(!m_check.loadModel(m_data.model)) {
        TXT_WARN("Failed to load ONNX model!");
    }
    QDir dir_input(m_data.dir_input);
    if(!dir_input.exists()) {
        TXT_WARN(QString("dir input not exist: %1").arg(m_data.dir_input));
        return;
    }
    QDir dir_output(m_data.dir_output);
    if(!dir_output.exists()) {
        TXT_WARN(QString("dir input not exist: %1").arg(m_data.dir_output));
        return;
    }
    QFileInfoList files = dir_input.entryInfoList(QStringList() << "*.jpg" << "*.png" << "*.jpeg", QDir::Files);

    for (const QFileInfo& fileInfo : files) {
        QString inputPath = fileInfo.absoluteFilePath();

        QString relativePath = fileInfo.absoluteFilePath().mid(m_data.dir_input.length());
        QString outputPath = m_data.dir_output + relativePath;

        QDir outputSubDir(QFileInfo(outputPath).absolutePath());
        if (!outputSubDir.exists()) {
            outputSubDir.mkpath(".");
        }
        bool bOk = m_check.checkOnePicture(inputPath, outputPath);
        if(bOk) {
            TXT_INFO(QString("Operate success %1 => %2").arg(inputPath, outputPath));
        }
        else {
            TXT_WARN(QString("Operate failed %1 => %2").arg(inputPath, outputPath));
        }
        QApplication::processEvents();
    }
    TXT_INFO(QString("Travel director %1 finished!").arg(m_data.dir_input));
}

void WidgetTest::on_btnStartTest_clicked()
{
    getUiData();
    save2Cfg();
    testOnnx();
}

void WidgetTest::on_sBoxModelWidth_editingFinished()
{
    m_check.setModelImgSize(ui->sBoxModelWidth->value(), 0);
}

void WidgetTest::on_sBoxModelHeight_editingFinished()
{
    m_check.setModelImgSize(0, ui->sBoxModelHeight->value());
}
