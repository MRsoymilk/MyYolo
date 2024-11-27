#include "widgettest.h"
#include "ui_widgettest.h"

#include <QFileDialog>
#include <QDir>

#include "../include/funcdef.h"
#include "../include/global.h"
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
    OPEN_FILE_BTN(ui->tBtnModelOnnx, ui->lEditModelOnnx);
    OPEN_FILE_BTN(ui->tBtnModelPt, ui->lEditModelPt);
    OPEN_FOLDER_BTN(ui->tBtnDirInput, ui->lEditDirInput);
    OPEN_FOLDER_BTN(ui->tBtnDirOutput, ui->lEditDirOutput);
    m_process = new QProcess();
    connect(m_process, &QProcess::readyReadStandardOutput, this, &WidgetTest::onProcessOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &WidgetTest::onProcessError);
    connect(m_process, &QProcess::finished, this, &WidgetTest::onProcessFinished);
}

void WidgetTest::getUiData() {
    m_data.model_height = ui->sBoxModelHeight->value();
    m_data.model_width = ui->sBoxModelWidth->value();
    m_data.model_onnx = ui->lEditModelOnnx->text();
    m_data.model_pt = ui->lEditModelPt->text();
    m_data.threshold_cfd = ui->doubleSpinBoxThresholdCfd->value();
    m_data.threshold_nms = ui->doubleSpinBoxThresholdNMS->value();
    m_data.dir_input = ui->lEditDirInput->text();
    m_data.dir_output = ui->lEditDirOutput->text();
}

void WidgetTest::getCfgData() {
    m_data.model_height = MY_SETTING.getValue(CFG_GROUP_TEST, CFG_TEST_MODEL_HEIGHT).toInt();
    m_data.model_width = MY_SETTING.getValue(CFG_GROUP_TEST, CFG_TEST_MODEL_WIDTH).toInt();
    m_data.model_onnx = MY_SETTING.getValue(CFG_GROUP_TEST, CFG_TEST_MODEL_ONNX);
    m_data.model_pt = MY_SETTING.getValue(CFG_GROUP_TEST, CFG_TEST_MODEL_PT);
    m_data.threshold_cfd = MY_SETTING.getValue(CFG_GROUP_TEST, CFG_TEST_THRESHOLD_CFD).toDouble();
    m_data.threshold_nms = MY_SETTING.getValue(CFG_GROUP_TEST, CFG_TEST_THRESHOLD_NMS).toDouble();
    m_data.dir_input = MY_SETTING.getValue(CFG_GROUP_TEST, CFG_TEST_DIR_INPUT);
    m_data.dir_output = MY_SETTING.getValue(CFG_GROUP_TEST, CFG_TEST_DIR_OUTPUT);
}

void WidgetTest::save2Cfg() {
    MY_SETTING.setValue(CFG_GROUP_TEST, CFG_TEST_MODEL_HEIGHT, QString::number(m_data.model_height));
    MY_SETTING.setValue(CFG_GROUP_TEST, CFG_TEST_MODEL_WIDTH, QString::number(m_data.model_width));
    MY_SETTING.setValue(CFG_GROUP_TEST, CFG_TEST_MODEL_ONNX, m_data.model_onnx);
    MY_SETTING.setValue(CFG_GROUP_TEST, CFG_TEST_MODEL_PT, m_data.model_pt);
    MY_SETTING.setValue(CFG_GROUP_TEST,
                        CFG_TEST_THRESHOLD_CFD,
                        QString::number(m_data.threshold_cfd));
    MY_SETTING.setValue(CFG_GROUP_TEST,
                        CFG_TEST_THRESHOLD_NMS,
                        QString::number(m_data.threshold_nms));
    MY_SETTING.setValue(CFG_GROUP_TEST, CFG_TEST_DIR_INPUT, m_data.dir_input);
    MY_SETTING.setValue(CFG_GROUP_TEST, CFG_TEST_DIR_OUTPUT, m_data.dir_output);
    MY_LOG_INFO("config save: Group[{}]", CFG_GROUP_TEST);
    MY_LOG_INFO("{}: {}", CFG_TEST_MODEL_HEIGHT, m_data.model_height);
    MY_LOG_INFO("{}: {}", CFG_TEST_MODEL_WIDTH, m_data.model_width);
    MY_LOG_INFO("{}: {}", CFG_TEST_THRESHOLD_CFD, m_data.threshold_cfd);
    MY_LOG_INFO("{}: {}", CFG_TEST_THRESHOLD_NMS, m_data.threshold_nms);
    MY_LOG_INFO("{}: {}", CFG_TEST_MODEL_ONNX, m_data.model_onnx);
    MY_LOG_INFO("{}: {}", CFG_TEST_MODEL_PT, m_data.model_pt);
    MY_LOG_INFO("{}: {}", CFG_TEST_DIR_INPUT, m_data.dir_input);
    MY_LOG_INFO("{}: {}", CFG_TEST_DIR_OUTPUT, m_data.dir_output);
}

void WidgetTest::show2Ui() {
    ui->sBoxModelHeight->setValue(m_data.model_height);
    ui->sBoxModelWidth->setValue(m_data.model_width);
    ui->doubleSpinBoxThresholdCfd->setValue(m_data.threshold_cfd);
    ui->doubleSpinBoxThresholdNMS->setValue(m_data.threshold_nms);
    ui->lEditModelOnnx->setText(m_data.model_onnx);
    ui->lEditModelPt->setText(m_data.model_pt);
    ui->lEditDirInput->setText(m_data.dir_input);
    ui->lEditDirOutput->setText(m_data.dir_output);
}

void WidgetTest::testPt() {
    QStringList arguments_detect{
        GLOBAL.SCRIPT_YOLO_DETECT,
        "--weights", m_data.model_pt,
        "--source", m_data.dir_input,
        "--project", m_data.dir_output,
        "--imgsz", QString("[%1, %2]").arg(m_data.model_height, m_data.model_width),
        "--conf-thres", QString::number(m_data.threshold_cfd),
        "--iou-thres", QString::number(m_data.threshold_nms)
    };
    TXT_INFO(QString("Script: %1").arg(arguments_detect.join(' ')));
    runScript(arguments_detect);
}

void WidgetTest::runScript(const QStringList &arguments)
{
    m_process->startDetached(GLOBAL.PYTHON, arguments);
}

void WidgetTest::onProcessOutput()
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    if (process) {
        QByteArray output = process->readAllStandardOutput();
        TXT_INFO(QString::fromUtf8(output));
    }
}

void WidgetTest::onProcessError()
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    if (process) {
        QByteArray errorOutput = process->readAllStandardError();
        TXT_WARN(QString::fromUtf8(errorOutput));
    }
}

void WidgetTest::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
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


void WidgetTest::testOnnx() {
    if(!m_onnxCheck.loadModel(m_data.model_onnx)) {
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
        bool bOk = m_onnxCheck.checkOnePicture(inputPath, outputPath);
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
    if(ui->toolBoxModel->currentWidget() == ui->pageModelOnnx)
    {
        testOnnx();
    }
    else if (ui->toolBoxModel->currentWidget() == ui->pageModelPt) {
        testPt();
    }
}

void WidgetTest::on_sBoxModelWidth_editingFinished()
{
    m_onnxCheck.setModelImgSize(ui->sBoxModelWidth->value(), 0);
}

void WidgetTest::on_sBoxModelHeight_editingFinished()
{
    m_onnxCheck.setModelImgSize(0, ui->sBoxModelHeight->value());
}

void WidgetTest::on_doubleSpinBoxThresholdCfd_editingFinished()
{
    m_onnxCheck.setConfidence(ui->doubleSpinBoxThresholdCfd->value());
}

void WidgetTest::on_doubleSpinBoxThresholdNMS_editingFinished()
{
    m_onnxCheck.setNMS(ui->doubleSpinBoxThresholdNMS->value());
}

void WidgetTest::on_btnAddItem_clicked()
{
    int newRow = ui->tableWidgetClassEdit->rowCount();
    ui->tableWidgetClassEdit->insertRow(newRow);

    QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(newRow));
    ui->tableWidgetClassEdit->setItem(newRow, 0, idItem);

    QTableWidgetItem *classNameItem = new QTableWidgetItem();
    ui->tableWidgetClassEdit->setItem(newRow, 1, classNameItem);

    QPushButton *deleteButton = new QPushButton("Delete");
    ui->tableWidgetClassEdit->setCellWidget(newRow, 2, deleteButton);

    connect(deleteButton, &QPushButton::clicked, this, [this, deleteButton]() {
        int row = ui->tableWidgetClassEdit->indexAt(deleteButton->pos()).row();
        ui->tableWidgetClassEdit->removeRow(row);
    });
}

void WidgetTest::on_btnRemoveItem_clicked()
{
    int row = ui->tableWidgetClassEdit->currentRow();
    if (row >= 0) {
        ui->tableWidgetClassEdit->removeRow(row);
    }
}

void WidgetTest::on_tableWidgetClassEdit_cellDoubleClicked(int row, int column)
{
    ui->tableWidgetClassEdit->editItem(ui->tableWidgetClassEdit->item(row, column));
}
