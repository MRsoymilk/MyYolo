#include "widgetlabel.h"
#include "ui_widgetlabel.h"
#include "../include/keydef.h"
#include "../include/funcdef.h"
#include "../include/global.h"
#include "../util/mysetting.h"

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
    m_data.path_labelimg = MY_SETTING.getValue(CFG_GROUP_LABEL, CFG_LABEL_PATH_LABELIMG, GLOBAL.EXE_LABELIMG);
}

void WidgetLabel::save2Cfg()
{
    MY_SETTING.setValue(CFG_GROUP_LABEL, CFG_LABEL_PATH_LABELIMG, m_data.path_labelimg);
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
        TXT_WARN(QString("Failed to Start: %1").arg(program));
    } else {
        TXT_INFO(QString("Success start LabelImg"));
    }
}

void WidgetLabel::onReadyReadStandardOutput()
{
    // 获取并处理外部程序的标准输出
    QProcess *process = qobject_cast<QProcess *>(sender());
    QByteArray output = process->readAllStandardOutput();
    TXT_INFO(QString("LabelImg: %1").arg(output));
}

void WidgetLabel::onReadyReadStandardError()
{
    // 获取并处理外部程序的错误输出
    QProcess *process = qobject_cast<QProcess *>(sender());
    QByteArray error = process->readAllStandardError();
    TXT_WARN(error);
}

void WidgetLabel::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    // 处理外部程序的退出状态
    if (exitStatus == QProcess::NormalExit) {
        TXT_INFO("LabelImg success quit.");
    } else {
        TXT_WARN("LabelImg abnormal quit!");
    }
}
