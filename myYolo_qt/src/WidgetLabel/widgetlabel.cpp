#include "widgetlabel.h"

#include "../include/funcdef.h"
#include "../include/global.h"
#include "../include/keydef.h"
#include "ui_widgetlabel.h"

WidgetLabel::WidgetLabel(QWidget *parent) : QWidget(parent), ui(new Ui::WidgetLabel)
{
    ui->setupUi(this);
    initLabel();
}

void WidgetLabel::initLabel()
{
    m_data.path_labelimg = SETTING_GET(CFG_GROUP_LABEL, CFG_LABEL_PATH_LABELIMG, GLOBAL.EXE_LABELIMG);
    ui->lEditLabelImg->setText(m_data.path_labelimg);
    REGISTER_FILE_BTN(ui->tBtnLabelImg, ui->lEditLabelImg);
}

WidgetLabel::~WidgetLabel() { delete ui; }

void WidgetLabel::on_btnStartLabel_clicked()
{
    m_data.path_labelimg = ui->lEditLabelImg->text();
    SETTING_SET(CFG_GROUP_LABEL, CFG_LABEL_PATH_LABELIMG, m_data.path_labelimg);
    bool success;
#ifdef Q_OS_WIN
    success = PROCESS_START_DETACH(GLOBAL.EXE_LABELIMG, {});
#endif
#ifdef Q_OS_UNIX
    success = PROCESS_START_DETACH(GLOBAL.PYTHON, {GLOBAL.EXE_LABELIMG});
#endif
    if (!success)
    {
        WIDGET_LOG_WARN(QString("Failed to Start: %1").arg(GLOBAL.EXE_LABELIMG));
    }
    else
    {
        WIDGET_LOG_INFO(QString("Success start LabelImg"));
    }
}
