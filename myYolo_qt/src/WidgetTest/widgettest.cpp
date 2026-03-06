#include "widgettest.h"

#include <QDir>
#include <QFileDialog>

#include "../include/funcdef.h"
#include "../include/global.h"
#include "../include/keydef.h"
#include "ui_widgettest.h"

WidgetTest::WidgetTest(QWidget *parent) : QWidget(parent), ui(new Ui::WidgetTest)
{
    ui->setupUi(this);
    initTest();
}

WidgetTest::~WidgetTest() { delete ui; }

void setClassList(QTableWidget *widget, const QStringList &classList)
{
    widget->setRowCount(classList.size());
    for (int row = 0; row < classList.size(); ++row)
    {
        if (classList.at(row).isEmpty())
        {
            continue;
        }
        widget->setItem(row, 0, new QTableWidgetItem(QString::number(row)));
        widget->setItem(row, 1, new QTableWidgetItem(classList.at(row)));
    }
}

void WidgetTest::initTest()
{
    m_data.model_height = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_MODEL_HEIGHT).toInt();
    m_data.model_width = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_MODEL_WIDTH).toInt();
    m_data.model_onnx = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_MODEL_ONNX);
    m_data.model_pt = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_MODEL_PT);
    m_data.threshold_cfd = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_THRESHOLD_CFD).toDouble();
    m_data.threshold_nms = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_THRESHOLD_NMS).toDouble();
    m_data.dir_input = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_DIR_INPUT);
    m_data.dir_output = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_DIR_OUTPUT);
    m_data.name = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_NAME);
    m_data.save_txt = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_SAVE_TXT).toInt();
    m_data.save_csv = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_SAVE_CSV).toInt();
    m_data.save_conf = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_SAVE_CONF).toInt();
    m_data.save_crop = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_SAVE_CROP).toInt();
    m_data.line_thickness = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_LINE_THICKNESS).toInt();
    m_data.view_img = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_VIEW_IMG).toInt();
    QStringList classes = SETTING_GET(CFG_GROUP_TEST, CFG_TEST_CLASSES).split(',');
    classes.removeAll("");
    m_data.classes = classes;
    ui->sBoxModelHeight->setValue(m_data.model_height);
    ui->sBoxModelWidth->setValue(m_data.model_width);
    ui->doubleSpinBoxThresholdCfd->setValue(m_data.threshold_cfd);
    ui->doubleSpinBoxThresholdNMS->setValue(m_data.threshold_nms);
    ui->lEditModelOnnx->setText(m_data.model_onnx);
    ui->lEditModelPt->setText(m_data.model_pt);
    ui->lEditDirInput->setText(m_data.dir_input);
    ui->lEditDirOutput->setText(m_data.dir_output);
    ui->lEditName->setText(m_data.name);
    ui->checkBoxSaveTxt->setCheckState(m_data.save_txt ? Qt::Checked : Qt::Unchecked);
    ui->checkBoxSaveCsv->setCheckState(m_data.save_csv ? Qt::Checked : Qt::Unchecked);
    ui->checkBoxSaveConf->setCheckState(m_data.save_conf ? Qt::Checked : Qt::Unchecked);
    ui->checkBoxSaveCrop->setCheckState(m_data.save_crop ? Qt::Checked : Qt::Unchecked);
    ui->spinBoxLineThickness->setValue(m_data.line_thickness);
    ui->checkBoxViewImg->setCheckState(m_data.view_img ? Qt::Checked : Qt::Unchecked);
    setClassList(ui->tableWidgetClassEdit, m_data.classes);
    REGISTER_FILE_BTN(ui->tBtnModelOnnx, ui->lEditModelOnnx);
    REGISTER_FILE_BTN(ui->tBtnModelPt, ui->lEditModelPt);
    REGISTER_FOLDER_BTN(ui->tBtnDirInput, ui->lEditDirInput);
    REGISTER_FOLDER_BTN(ui->tBtnDirOutput, ui->lEditDirOutput);
    on_toolBoxModel_currentChanged(TEST_SWITCH::YOLOV5_DETECT);
}

QStringList getClassList(QTableWidget *widget)
{
    QStringList list{};
    int rowCount = widget->rowCount();
    for (int i = 0; i < rowCount; ++i)
    {
        QString cellValue = widget->item(i, 1)->text();
        list.append(cellValue);
    }
    return list;
}

void WidgetTest::testPt()
{
    QStringList arguments_detect{GLOBAL.SCRIPT_YOLO_DETECT,
                                 "--weights",
                                 m_data.model_pt,
                                 "--source",
                                 m_data.dir_input,
                                 "--project",
                                 m_data.dir_output,
                                 "--name",
                                 m_data.name,
                                 "--line-thickness",
                                 QString::number(m_data.line_thickness),
                                 "--imgsz",
                                 QString::number(m_data.model_height),
                                 QString::number(m_data.model_width),
                                 "--conf-thres",
                                 QString::number(m_data.threshold_cfd),
                                 "--iou-thres",
                                 QString::number(m_data.threshold_nms)};
    if (m_data.view_img)
    {
        arguments_detect << "--view-img";
    }
    if (m_data.save_txt)
    {
        arguments_detect << "--save-txt";
    }
    if (m_data.save_csv)
    {
        arguments_detect << "--save-csv";
    }
    if (m_data.save_conf)
    {
        arguments_detect << "--save-conf";
    }
    if (m_data.save_crop)
    {
        arguments_detect << "--save-crop";
    }
    WIDGET_LOG_INFO(QString("Script: %1").arg(arguments_detect.join(' ')));
    LOG_INFO("Test .pt: {}", arguments_detect);
    PROCESS_START_ATTACH(GLOBAL.PYTHON, arguments_detect);
}

void WidgetTest::testOnnx()
{
    if (!m_onnxCheck.loadModel(m_data.model_onnx))
    {
        WIDGET_LOG_WARN("Failed to load ONNX model!");
    }
    QDir dir_input(m_data.dir_input);
    if (!dir_input.exists())
    {
        WIDGET_LOG_WARN(QString("dir input not exist: %1").arg(m_data.dir_input));
        return;
    }
    QDir dir_output(m_data.dir_output);
    if (!dir_output.exists())
    {
        WIDGET_LOG_WARN(QString("dir input not exist: %1").arg(m_data.dir_output));
        return;
    }

    m_onnxCheck.setClasses(m_data.classes);
    m_onnxCheck.setModelImgSize(m_data.model_width, m_data.model_height);
    m_onnxCheck.setNMS(m_data.threshold_nms);
    m_onnxCheck.setConfidence(m_data.threshold_cfd);

    QFileInfoList files = dir_input.entryInfoList(QStringList() << "*.jpg" << "*.png" << "*.jpeg", QDir::Files);

    for (const QFileInfo &fileInfo : files)
    {
        QString inputPath = fileInfo.absoluteFilePath();

        QString relativePath = fileInfo.absoluteFilePath().mid(m_data.dir_input.length());
        QString outputPath = m_data.dir_output + relativePath;

        QDir outputSubDir(QFileInfo(outputPath).absolutePath());
        if (!outputSubDir.exists())
        {
            outputSubDir.mkpath(".");
        }
        bool bOk = m_onnxCheck.checkOnePicture(inputPath, outputPath);
        if (bOk)
        {
            WIDGET_LOG_INFO(QString("Operate success %1 => %2").arg(inputPath, outputPath));
        }
        else
        {
            WIDGET_LOG_WARN(QString("Operate failed %1 => %2").arg(inputPath, outputPath));
        }
        QApplication::processEvents();
    }
    WIDGET_LOG_INFO(QString("Travel director %1 finished!").arg(m_data.dir_input));
}

void WidgetTest::on_btnStartTest_clicked()
{
    m_data.model_height = ui->sBoxModelHeight->value();
    m_data.model_width = ui->sBoxModelWidth->value();
    m_data.model_onnx = ui->lEditModelOnnx->text();
    m_data.model_pt = ui->lEditModelPt->text();
    m_data.threshold_cfd = ui->doubleSpinBoxThresholdCfd->value();
    m_data.threshold_nms = ui->doubleSpinBoxThresholdNMS->value();
    m_data.dir_input = ui->lEditDirInput->text();
    m_data.dir_output = ui->lEditDirOutput->text();
    m_data.name = ui->lEditName->text();
    m_data.save_txt = ui->checkBoxSaveTxt->isChecked() ? 1 : 0;
    m_data.save_csv = ui->checkBoxSaveCsv->isChecked() ? 1 : 0;
    m_data.save_conf = ui->checkBoxSaveConf->isChecked() ? 1 : 0;
    m_data.save_crop = ui->checkBoxSaveCrop->isChecked() ? 1 : 0;
    m_data.line_thickness = ui->spinBoxLineThickness->value();
    m_data.view_img = ui->checkBoxViewImg->isChecked() ? 1 : 0;
    m_data.classes = getClassList(ui->tableWidgetClassEdit);
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_MODEL_HEIGHT, QString::number(m_data.model_height));
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_MODEL_WIDTH, QString::number(m_data.model_width));
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_MODEL_ONNX, m_data.model_onnx);
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_MODEL_PT, m_data.model_pt);
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_THRESHOLD_CFD, QString::number(m_data.threshold_cfd));
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_THRESHOLD_NMS, QString::number(m_data.threshold_nms));
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_DIR_INPUT, m_data.dir_input);
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_DIR_OUTPUT, m_data.dir_output);
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_NAME, m_data.name);
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_SAVE_TXT, QString::number(m_data.save_txt));
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_SAVE_CSV, QString::number(m_data.save_csv));
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_SAVE_CONF, QString::number(m_data.save_conf));
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_SAVE_CROP, QString::number(m_data.save_crop));
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_LINE_THICKNESS, QString::number(m_data.line_thickness));
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_VIEW_IMG, QString::number(m_data.view_img));
    SETTING_SET(CFG_GROUP_TEST, CFG_TEST_CLASSES, m_data.classes.join(','));
    LOG_INFO("config save: Group[{}]", CFG_GROUP_TEST);
    LOG_INFO("{}: {}", CFG_TEST_MODEL_HEIGHT, m_data.model_height);
    LOG_INFO("{}: {}", CFG_TEST_MODEL_WIDTH, m_data.model_width);
    LOG_INFO("{}: {}", CFG_TEST_THRESHOLD_CFD, m_data.threshold_cfd);
    LOG_INFO("{}: {}", CFG_TEST_THRESHOLD_NMS, m_data.threshold_nms);
    LOG_INFO("{}: {}", CFG_TEST_MODEL_ONNX, m_data.model_onnx);
    LOG_INFO("{}: {}", CFG_TEST_MODEL_PT, m_data.model_pt);
    LOG_INFO("{}: {}", CFG_TEST_DIR_INPUT, m_data.dir_input);
    LOG_INFO("{}: {}", CFG_TEST_DIR_OUTPUT, m_data.dir_output);
    if (ui->toolBoxModel->currentWidget() == ui->pageModelOnnx)
    {
        testOnnx();
    }
    else if (ui->toolBoxModel->currentWidget() == ui->pageModelPt)
    {
        testPt();
    }
}

void WidgetTest::on_sBoxModelWidth_editingFinished() { m_onnxCheck.setModelImgSize(ui->sBoxModelWidth->value(), 0); }

void WidgetTest::on_sBoxModelHeight_editingFinished() { m_onnxCheck.setModelImgSize(0, ui->sBoxModelHeight->value()); }

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

    connect(deleteButton, &QPushButton::clicked, this,
            [this, deleteButton]()
            {
                int row = ui->tableWidgetClassEdit->indexAt(deleteButton->pos()).row();
                ui->tableWidgetClassEdit->removeRow(row);
            });
}

void WidgetTest::on_btnRemoveItem_clicked()
{
    int row = ui->tableWidgetClassEdit->currentRow();
    if (row >= 0)
    {
        ui->tableWidgetClassEdit->removeRow(row);
    }
}

void WidgetTest::on_tableWidgetClassEdit_cellDoubleClicked(int row, int column)
{
    ui->tableWidgetClassEdit->editItem(ui->tableWidgetClassEdit->item(row, column));
}

void WidgetTest::on_toolBoxModel_currentChanged(int index)
{
    if (index == TEST_SWITCH::YOLOV5_DETECT)
    {
        // yolov5 detect.py
        ui->checkBoxSaveConf->setVisible(true);
        ui->checkBoxSaveCrop->setVisible(true);
        ui->checkBoxSaveCsv->setVisible(true);
        ui->checkBoxSaveTxt->setVisible(true);
        ui->checkBoxViewImg->setVisible(true);
        ui->tableWidgetClassEdit->setVisible(false);
        ui->btnAddItem->setVisible(false);
        ui->btnRemoveItem->setVisible(false);
    }
    if (index == TEST_SWITCH::OPENCV_ONNX)
    {
        // opencv onnx
        ui->checkBoxSaveConf->setVisible(false);
        ui->checkBoxSaveCrop->setVisible(false);
        ui->checkBoxSaveCsv->setVisible(false);
        ui->checkBoxSaveTxt->setVisible(false);
        ui->checkBoxViewImg->setVisible(false);
        ui->tableWidgetClassEdit->setVisible(true);
        ui->btnAddItem->setVisible(true);
        ui->btnRemoveItem->setVisible(true);
    }
}
