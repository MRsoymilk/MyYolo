#include "widgetfilter.h"

#include "../include/funcdef.h"
#include "../include/global.h"
#include "../include/keydef.h"
#include "ui_widgetfilter.h"

WidgetFilter::WidgetFilter(QWidget *parent) : QWidget(parent), ui(new Ui::WidgetFilter)
{
    ui->setupUi(this);
    initFilter();
}

WidgetFilter::~WidgetFilter() { delete ui; }

void WidgetFilter::initFilter()
{
    m_data.dir_input = SETTING_GET(CFG_GROUP_FILTER, CFG_FILTER_DIR_INPUT);
    m_data.dir_output = SETTING_GET(CFG_GROUP_FILTER, CFG_FILTER_DIR_OUTPUT);
    m_data.threshold_ssim = SETTING_GET(CFG_GROUP_FILTER, CFG_FILTER_THRESHOLD_SSIM).toDouble();
    m_data.batch_ssim = SETTING_GET(CFG_GROUP_FILTER, CFG_FILTER_BATCH_SSIM).toInt();
    ui->lEditDirInput->setText(m_data.dir_input);
    ui->lEditDirOutput->setText(m_data.dir_output);
    REGISTER_FOLDER_BTN(ui->tBtnDirInput, ui->lEditDirInput);
    REGISTER_FOLDER_BTN(ui->tBtnDirOutput, ui->lEditDirOutput);
}

void WidgetFilter::on_btnStartFilter_clicked()
{
    m_data.dir_input = ui->lEditDirInput->text();
    m_data.dir_output = ui->lEditDirOutput->text();
    SETTING_SET(CFG_GROUP_FILTER, CFG_FILTER_DIR_INPUT, m_data.dir_input);
    SETTING_SET(CFG_GROUP_FILTER, CFG_FILTER_DIR_OUTPUT, m_data.dir_output);
    SETTING_SET(CFG_GROUP_FILTER, CFG_FILTER_THRESHOLD_SSIM, QString::number(m_data.threshold_ssim));
    SETTING_SET(CFG_GROUP_FILTER, CFG_FILTER_BATCH_SSIM, QString::number(m_data.batch_ssim));
    if (m_data.dir_input.isEmpty())
    {
        SHOW_MSGBOX_CRITICAL("dir input cannot be empty!");
        return;
    }
    if (m_data.dir_output.isEmpty())
    {
        SHOW_MSGBOX_CRITICAL("dir output cannot be empty!");
        return;
    }
}
