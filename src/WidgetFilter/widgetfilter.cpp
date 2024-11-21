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
}

void WidgetFilter::getCfgData()
{
    m_data.dir_input = MY_SETTING.getValue(CFG_GROUP_FILTER, CFG_FILTER_DIR_INPUT);
    m_data.dir_output = MY_SETTING.getValue(CFG_GROUP_FILTER, CFG_FILTER_DIR_OUTPUT);
    m_data.is_ssim = MY_SETTING.getValue(CFG_GROUP_FILTER, CFG_FILTER_SSIM).toInt();
}

void WidgetFilter::save2Cfg()
{
    MY_SETTING.setValue(CFG_GROUP_FILTER, CFG_FILTER_DIR_INPUT, m_data.dir_input);
    MY_SETTING.setValue(CFG_GROUP_FILTER, CFG_FILTER_DIR_OUTPUT, m_data.dir_output);
    MY_SETTING.setValue(CFG_GROUP_FILTER, CFG_FILTER_SSIM, QString::number(m_data.is_ssim));
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
}

void WidgetFilter::runScript(const QStringList &arguments)
{
    m_process->startDetached(GLOBAL.PYTHON, arguments);
}

void WidgetFilter::callMoveSimilarImgs()
{
    QStringList arguments_move_similar{GLOBAL.SCRIPT_MOVE_SIMILAR_IMG,
                                       "--input_folder",
                                       m_data.dir_input,
                                       "--output_folder",
                                       m_data.dir_output};
    runScript(arguments_move_similar);
}

void WidgetFilter::on_btnStartFilter_clicked()
{
    getUiData();
    save2Cfg();
    if (m_data.is_ssim) {
        callMoveSimilarImgs();
    }
}
