#include "widgetadddata.h"

#include <qcombobox.h>

#include <QFileDialog>
#include <QProcess>

#include "../include/funcdef.h"
#include "../include/global.h"
#include "../include/keydef.h"
#include "ui_widgetadddata.h"

WidgetAddData::WidgetAddData(QWidget *parent) : QWidget(parent), ui(new Ui::WidgetAddData)
{
    ui->setupUi(this);
    initAddData();
}

WidgetAddData::~WidgetAddData() { delete ui; }

QStringList getTagsList(QTableWidget *widget)
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

void setTagsList(QTableWidget *widget, const QStringList &classList)
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

void WidgetAddData::initAddData()
{
    m_data.dir_img = SETTING_GET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_DIR_INPUT);
    QStringList tags = SETTING_GET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_LIST_TAG).split(',');
    tags.removeAll("");
    m_data.list_tag = tags;
    m_data.train = SETTING_GET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_TRAIN).toDouble();
    m_data.valid = SETTING_GET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_VALID).toDouble();
    m_data.test = SETTING_GET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_TEST).toDouble();
    m_data.dir_output_divide = SETTING_GET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_DIR_OUTPUT_DIVIDE);
    m_data.is_shuffle = SETTING_GET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_IS_SHUFFLE).toInt();
    m_data.is_rename = SETTING_GET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_IS_RENAME).toInt();
    m_data.rename_prefix = SETTING_GET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RENAME_PREFIX);
    m_data.rename_suffix = SETTING_GET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RENAME_SUFFIX);
    m_data.is_rename_no_tag = SETTING_GET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_IS_RENAME_NOTAG).toInt();
    m_data.re_notag_prefix = SETTING_GET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RE_NOTAG_PREFIX);
    m_data.re_notag_suffix = SETTING_GET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RE_NOTAG_SUFFIX);
    ui->lEditDirInput->setText(m_data.dir_img);
    setTagsList(ui->tableWidgetTag, m_data.list_tag);
    ui->doubleSpinBoxTrain->setValue(m_data.train);
    ui->doubleSpinBoxValid->setValue(m_data.valid);
    ui->doubleSpinBoxTest->setValue(m_data.test);
    ui->lEditDirOutputDivide->setText(m_data.dir_output_divide);
    ui->checkBoxShuffle->setCheckState(m_data.is_shuffle ? Qt::Checked : Qt::Unchecked);
    ui->checkBoxRename->setCheckState(m_data.is_rename ? Qt::Checked : Qt::Unchecked);
    ui->lEditRenamePrefix->setText(m_data.rename_prefix);
    ui->lEditRenameSuffix->setText(m_data.rename_suffix);
    ui->checkBoxReNoTag->setCheckState(m_data.is_rename_no_tag ? Qt::Checked : Qt::Unchecked);
    ui->lEditReNoTagPrefix->setText(m_data.re_notag_prefix);
    ui->lEditReNoTagSuffix->setText(m_data.re_notag_suffix);
    REGISTER_FOLDER_BTN(ui->tBtnDirInput, ui->lEditDirInput);
    REGISTER_FOLDER_BTN(ui->tBtnDirOutputDivide, ui->lEditDirOutputDivide);
}

void WidgetAddData::callXml2Txt()
{
    // python xml2txt.py --class_names car,person --input_folder
    // /path/to/input_folder --output_folder /path/to/output_folder
    QStringList arguments_xml2txt{GLOBAL.SCRIPT_XML2TXT, "--class_name", m_data.list_tag.join(','),
                                  "--input_folder",      m_data.dir_img, "--output_folder",
                                  m_data.dir_img};
    WIDGET_LOG_INFO(QString("Script: %1").arg(arguments_xml2txt.join(' ')));
    PROCESS_START_ATTACH(GLOBAL.PYTHON, arguments_xml2txt);
}

void WidgetAddData::callNoXml2Txt()
{
    // python no_xml2txt.py --input_folder /path/to/input_folder --output_folder
    // /path/to/output_folder
    QStringList arguments_no_xml2txt{GLOBAL.SCRIPT_NO_XML2TXT, "--input_folder", m_data.dir_img, "--output_folder",
                                     m_data.dir_img};
    WIDGET_LOG_INFO(QString("Script: %1").arg(arguments_no_xml2txt.join(' ')));
    PROCESS_START_ATTACH(GLOBAL.PYTHON, arguments_no_xml2txt);
}

void WidgetAddData::callRename()
{
    QStringList arguments_rename{GLOBAL.SCRIPT_RENAME, "--input_folder", m_data.dir_img};
    if (!m_data.rename_prefix.isEmpty())
    {
        arguments_rename << "--prefix" << m_data.rename_prefix;
    }
    if (!m_data.rename_suffix.isEmpty())
    {
        arguments_rename << "--suffix" << m_data.rename_suffix;
    }
    WIDGET_LOG_INFO(QString("Script: %1").arg(arguments_rename.join(' ')));
    PROCESS_START_ATTACH(GLOBAL.PYTHON, arguments_rename);
}

void WidgetAddData::callReNoTag()
{
    QStringList arguments_re_notag{GLOBAL.SCRIPT_RE_NOTAG, "--input_folder", m_data.dir_img};
    if (!m_data.re_notag_prefix.isEmpty())
    {
        arguments_re_notag << "--prefix" << m_data.re_notag_prefix;
    }
    if (!m_data.re_notag_suffix.isEmpty())
    {
        arguments_re_notag << "--suffix" << m_data.re_notag_suffix;
    }
    WIDGET_LOG_INFO(QString("Script: %1").arg(arguments_re_notag.join(' ')));
    PROCESS_START_ATTACH(GLOBAL.PYTHON, arguments_re_notag);
}

void WidgetAddData::callSpliteDataset()
{
    QStringList arguments_splitedata{GLOBAL.SCRIPT_SPLIT_DATASET,
                                     "--dir_in",
                                     m_data.dir_img,
                                     "--dir_out",
                                     m_data.dir_output_divide,
                                     "--train-ratio",
                                     QString::number(m_data.train),
                                     "--val-ratio",
                                     QString::number(m_data.valid),
                                     "--test-ratio",
                                     QString::number(m_data.test),
                                     "--shuffle",
                                     QString::number(m_data.is_shuffle)};
    WIDGET_LOG_INFO(QString("Script: %1").arg(arguments_splitedata.join(' ')));
    PROCESS_START_ATTACH(GLOBAL.PYTHON, arguments_splitedata);
}

void WidgetAddData::on_btnStartAddData_clicked()
{
    m_data.dir_img = ui->lEditDirInput->text();
    m_data.list_tag = getTagsList(ui->tableWidgetTag);
    m_data.train = ui->doubleSpinBoxTrain->value();
    m_data.valid = ui->doubleSpinBoxValid->value();
    m_data.test = ui->doubleSpinBoxTest->value();
    m_data.dir_output_divide = ui->lEditDirOutputDivide->text();
    m_data.is_shuffle = ui->checkBoxShuffle->isChecked();
    m_data.is_rename = ui->checkBoxRename->isChecked();
    m_data.rename_prefix = ui->lEditRenamePrefix->text();
    m_data.rename_suffix = ui->lEditRenameSuffix->text();
    m_data.is_rename_no_tag = ui->checkBoxReNoTag->isChecked();
    m_data.re_notag_prefix = ui->lEditReNoTagPrefix->text();
    m_data.re_notag_suffix = ui->lEditReNoTagSuffix->text();
    SETTING_SET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_DIR_INPUT, m_data.dir_img);
    SETTING_SET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_LIST_TAG, m_data.list_tag.join(','));
    SETTING_SET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_TRAIN, QString::number(m_data.train));
    SETTING_SET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_VALID, QString::number(m_data.valid));
    SETTING_SET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_TEST, QString::number(m_data.test));
    SETTING_SET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_DIR_OUTPUT_DIVIDE, m_data.dir_output_divide);
    SETTING_SET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_IS_SHUFFLE, QString::number(m_data.is_shuffle));
    SETTING_SET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_IS_RENAME, QString::number(m_data.is_rename));
    SETTING_SET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RENAME_PREFIX, m_data.rename_prefix);
    SETTING_SET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RENAME_SUFFIX, m_data.rename_suffix);
    SETTING_SET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_IS_RENAME_NOTAG, QString::number(m_data.is_rename_no_tag));
    SETTING_SET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RE_NOTAG_PREFIX, m_data.re_notag_prefix);
    SETTING_SET(CFG_GROUP_ADD_DATA, CFG_ADD_DATA_RE_NOTAG_SUFFIX, m_data.re_notag_suffix);

    LOG_INFO("config save: Group[{}]", CFG_GROUP_ADD_DATA);
    LOG_INFO("{}: {}", CFG_ADD_DATA_DIR_INPUT, m_data.dir_img);
    LOG_INFO("{}: {}", CFG_ADD_DATA_LIST_TAG, m_data.list_tag);
    LOG_INFO("{}: {}", CFG_ADD_DATA_TRAIN, m_data.train);
    LOG_INFO("{}: {}", CFG_ADD_DATA_VALID, m_data.valid);
    LOG_INFO("{}: {}", CFG_ADD_DATA_TEST, m_data.test);
    LOG_INFO("{}: {}", CFG_ADD_DATA_DIR_OUTPUT_DIVIDE, m_data.dir_output_divide);
    LOG_INFO("{}: {}", CFG_ADD_DATA_IS_SHUFFLE, m_data.is_shuffle);
    LOG_INFO("{}: {}", CFG_ADD_DATA_IS_RENAME, m_data.is_rename);
    LOG_INFO("{}: {}", CFG_ADD_DATA_RENAME_PREFIX, m_data.rename_prefix);
    LOG_INFO("{}: {}", CFG_ADD_DATA_RENAME_SUFFIX, m_data.rename_suffix);
    LOG_INFO("{}: {}", CFG_ADD_DATA_IS_RENAME_NOTAG, m_data.is_rename_no_tag);
    LOG_INFO("{}: {}", CFG_ADD_DATA_RE_NOTAG_PREFIX, m_data.re_notag_prefix);
    LOG_INFO("{}: {}", CFG_ADD_DATA_RE_NOTAG_SUFFIX, m_data.re_notag_suffix);
    WIDGET_LOG_INFO("dir input: " + m_data.dir_img);
    WIDGET_LOG_INFO("dir output: " + m_data.dir_output_divide);
    WIDGET_LOG_INFO(QString("train: %1").arg(m_data.train));
    WIDGET_LOG_INFO(QString("valid: %1").arg(m_data.valid));
    WIDGET_LOG_INFO(QString("test: %1").arg(m_data.test));

    // pre check
    if (m_data.dir_img.isEmpty())
    {
        SHOW_MSGBOX_CRITICAL("data input can not be empty!");
        return;
    }
    if (m_data.dir_output_divide.isEmpty())
    {
        SHOW_MSGBOX_CRITICAL("data output divide can not be empty!");
        return;
    }
    const double epsilon = 1e-6;
    double val = qAbs((m_data.train + m_data.valid + m_data.test) - 1.0);
    if (val > epsilon)
    {
        SHOW_MSGBOX_CRITICAL("train + valid + test must be 1.0!");
        return;
    }

    // generate txt file: call xml2txt.py
    callXml2Txt();
    // generate no txt file: call no_xml2txt.py
    callNoXml2Txt();
    // reaname: call rename.py
    if (ui->checkBoxRename->isChecked())
    {
        if (ui->lEditRenamePrefix->text().isEmpty() && ui->lEditRenameSuffix->text().isEmpty())
        {
            WIDGET_LOG_WARN("empty prefix and suffix");
        }
        else
        {
            callRename();
        }
    }
    // rename: call re_notag.py
    if (ui->checkBoxReNoTag->isChecked())
    {
        if (ui->lEditReNoTagPrefix->text().isEmpty() && ui->lEditReNoTagSuffix->text().isEmpty())
        {
            WIDGET_LOG_WARN("empty prefix and suffix");
        }
        else
        {
            callReNoTag();
        }
    }
    // add train.txt, test.txt, val.txt
    callSpliteDataset();
}

void WidgetAddData::on_btnAddTag_clicked()
{
    int newRow = ui->tableWidgetTag->rowCount();
    ui->tableWidgetTag->insertRow(newRow);

    QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(newRow));
    ui->tableWidgetTag->setItem(newRow, 0, idItem);

    QTableWidgetItem *classNameItem = new QTableWidgetItem();
    ui->tableWidgetTag->setItem(newRow, 1, classNameItem);

    QPushButton *deleteButton = new QPushButton("Delete");
    ui->tableWidgetTag->setCellWidget(newRow, 2, deleteButton);

    connect(deleteButton, &QPushButton::clicked, this,
            [this, deleteButton]()
            {
                int row = ui->tableWidgetTag->indexAt(deleteButton->pos()).row();
                ui->tableWidgetTag->removeRow(row);
            });
}

void WidgetAddData::on_btnRemoveTag_clicked()
{
    int row = ui->tableWidgetTag->currentRow();
    if (row >= 0)
    {
        ui->tableWidgetTag->removeRow(row);
    }
}

void WidgetAddData::on_tableWidgetTag_cellDoubleClicked(int row, int column)
{
    ui->tableWidgetTag->editItem(ui->tableWidgetTag->item(row, column));
}
