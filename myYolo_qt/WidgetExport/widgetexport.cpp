#include "widgetexport.h"

#include "../include/funcdef.h"
#include "../include/global.h"
#include "../include/keydef.h"
#include "ui_widgetexport.h"

WidgetExport::WidgetExport(QWidget *parent) : QWidget(parent), ui(new Ui::WidgetExport)
{
    ui->setupUi(this);
    initExport();
}

WidgetExport::~WidgetExport() { delete ui; }

void WidgetExport::initExport()
{
    getCfgData();
    show2Ui();
    OPEN_FILE_BTN(ui->tBtnPtModel, ui->lEditPtModel);
}

void WidgetExport::getCfgData()
{
    m_data.list_type = SETTING_GET(CFG_GROUP_EXPORT, CFG_EXPORT_TYPE).split(',');
    m_data.type = m_data.list_type.first();
    m_data.pt_model = SETTING_GET(CFG_GROUP_EXPORT, CFG_EXPORT_PT_MODEL);
    m_data.model_h = SETTING_GET(CFG_GROUP_EXPORT, CFG_EXPORT_MODEL_H).toInt();
    m_data.model_w = SETTING_GET(CFG_GROUP_EXPORT, CFG_EXPORT_MODEL_W).toInt();
    m_data.model_batch = SETTING_GET(CFG_GROUP_EXPORT, CFG_EXPORT_MODEL_BATCH).toInt();
}

QStringList getTypeList(QComboBox *box)
{
    QStringList list = {};
    for (int i = 0; i < box->count(); ++i)
    {
        list.append(box->itemText(i));
    }
    return list;
}

void WidgetExport::save2Cfg()
{
    SETTING_SET(CFG_GROUP_EXPORT, CFG_EXPORT_TYPE, m_data.list_type.join(','));
    SETTING_SET(CFG_GROUP_EXPORT, CFG_EXPORT_PT_MODEL, m_data.pt_model);
    SETTING_SET(CFG_GROUP_EXPORT, CFG_EXPORT_MODEL_H, QString::number(m_data.model_h));
    SETTING_SET(CFG_GROUP_EXPORT, CFG_EXPORT_MODEL_W, QString::number(m_data.model_w));
    SETTING_SET(CFG_GROUP_EXPORT, CFG_EXPORT_MODEL_BATCH, QString::number(m_data.model_batch));
    LOG_INFO("config save: Group[{}]", CFG_GROUP_EXPORT);
    LOG_INFO("{}: {}", CFG_EXPORT_TYPE, m_data.list_type);
    LOG_INFO("{}: {}", CFG_EXPORT_PT_MODEL, m_data.pt_model);
    LOG_INFO("{}: {}", CFG_EXPORT_MODEL_H, m_data.model_h);
    LOG_INFO("{}: {}", CFG_EXPORT_MODEL_W, m_data.model_w);
    LOG_INFO("{}: {}", CFG_EXPORT_MODEL_BATCH, m_data.model_batch);
}

void WidgetExport::show2Ui()
{
    ui->comboBoxType->clear();
    ui->comboBoxType->addItems(m_data.list_type);
    ui->lEditPtModel->setText(m_data.pt_model);
    ui->sBoxModelH->setValue(m_data.model_h);
    ui->sBoxModelW->setValue(m_data.model_w);
    ui->sBoxModelBatch->setValue(m_data.model_batch);
}

void WidgetExport::getUiData()
{
    m_data.list_type = getTypeList(ui->comboBoxType);
    m_data.type = ui->comboBoxType->currentText();
    m_data.pt_model = ui->lEditPtModel->text();
    m_data.model_h = ui->sBoxModelH->value();
    m_data.model_w = ui->sBoxModelW->value();
    m_data.model_batch = ui->sBoxModelBatch->value();
}

void WidgetExport::on_btnStartExport_clicked()
{
    getUiData();
    save2Cfg();
    QStringList arguments_export{GLOBAL.SCRIPT_YOLO_EXPORT,
                                 "--weights",
                                 m_data.pt_model,
                                 "--batch-size",
                                 QString::number(m_data.model_batch),
                                 "--imgsz",
                                 QString::number(m_data.model_h),
                                 QString::number(m_data.model_w),
                                 "--include",
                                 m_data.type};
    PROCESS_START_ATTACH(GLOBAL.PYTHON, arguments_export);
}

void WidgetExport::on_btnAddType_clicked()
{
    QString type = ui->lEditAddType->text().trimmed();
    if (type.isEmpty())
    {
        SHOW_MSGBOX_WARNING("type cannot be empty!");
        return;
    }
    if (ui->comboBoxType->findText(type) != -1)
    {
        SHOW_MSGBOX_WARNING("type is already added!");
        return;
    }
    ui->comboBoxType->addItem(type);
    m_data.list_type = getTypeList(ui->comboBoxType);
}

void WidgetExport::on_comboBoxType_currentTextChanged(const QString &type) { m_data.type = type; }
