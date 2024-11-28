#include "widgetlabel.h"
#include "ui_widgetlabel.h"
#include "../include/keydef.h"
#include "../include/funcdef.h"
#include "../include/global.h"

#include <QProcess>

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
    m_process = new QProcess(this);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &WidgetLabel::onReadyReadStandardOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &WidgetLabel::onReadyReadStandardError);
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
    QString program = GLOBAL.EXE_LABELIMG;
    bool succss = m_process->startDetached(program);

    if (!succss) {
        WIDGET_LOG_WARN(QString("Failed to Start: %1").arg(program));
    } else {
        WIDGET_LOG_INFO(QString("Success start LabelImg"));
    }
}

void WidgetLabel::onReadyReadStandardOutput()
{
    QProcess *process = qobject_cast<QProcess *>(sender());
    QByteArray output = process->readAllStandardOutput();
    WIDGET_LOG_INFO(QString("LabelImg: %1").arg(output));
}

void WidgetLabel::onReadyReadStandardError()
{
    QProcess *process = qobject_cast<QProcess *>(sender());
    QByteArray error = process->readAllStandardError();
    WIDGET_LOG_WARN(error);
}

void WidgetLabel::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit) {
        WIDGET_LOG_TRACE("LabelImg success quit.");
    } else {
        WIDGET_LOG_WARN("LabelImg abnormal quit!");
    }
}
