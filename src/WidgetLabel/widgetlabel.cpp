#include "widgetlabel.h"
#include "ui_widgetlabel.h"
#include "../include/keydef.h"
#include "../include/funcdef.h"
#include "../include/global.h"

WidgetLabel::WidgetLabel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetLabel)
{
    ui->setupUi(this);
    initLabel();
}

void WidgetLabel::initLabel() {
    getCfgData();
    show2Ui();
    OPEN_FILE_BTN(ui->tBtnLabelImg, ui->lEditLabelImg);
}

WidgetLabel::~WidgetLabel()
{
    delete ui;
}

void WidgetLabel::getCfgData()
{
    m_data.path_labelimg = SETTING_GET(CFG_GROUP_LABEL, CFG_LABEL_PATH_LABELIMG, GLOBAL.EXE_LABELIMG);
}

void WidgetLabel::save2Cfg()
{
    SETTING_SET(CFG_GROUP_LABEL, CFG_LABEL_PATH_LABELIMG, m_data.path_labelimg);
}

void WidgetLabel::show2Ui()
{
    ui->lEditLabelImg->setText(m_data.path_labelimg);
}

void WidgetLabel::getUiData()
{
    m_data.path_labelimg = ui->lEditLabelImg->text();
}

void WidgetLabel::on_btnStartLabel_clicked()
{
    bool success;
#ifdef Q_OS_WIN
    success = PROCESS_START_DETACH(GLOBAL.EXE_LABELIMG, {});
#endif
#ifdef Q_OS_UNIX
    success = PROCESS_START_DETACH(GLOBAL.PYTHON, {GLOBAL.EXE_LABELIMG});
#endif
    if (!success) {
        WIDGET_LOG_WARN(QString("Failed to Start: %1").arg(GLOBAL.EXE_LABELIMG));
    } else {
        WIDGET_LOG_INFO(QString("Success start LabelImg"));
    }
}
