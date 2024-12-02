#ifndef WIDGETADDDATA_H
#define WIDGETADDDATA_H

#include <QWidget>
#include <QLineEdit>
#include "../common/confighandler.h"

namespace Ui {
class WidgetAddData;
}

class WidgetAddData : public QWidget, public ConfigHandler
{
    Q_OBJECT

public:
    explicit WidgetAddData(QWidget *parent = nullptr);
    ~WidgetAddData();
    struct ini_add_data {
        QString dir_img = "";
        QStringList list_tag;
        double train = 0.0;
        double valid = 0.0;
        double test = 0.0;
        QString dir_output_divide = "";
        int is_shuffle;
        int is_rename;
        QString rename_prefix;
        QString rename_suffix;
        int is_rename_no_tag;
        QString re_notag_prefix;
        QString re_notag_suffix;
    };

private:
    void initAddData();
    void getCfgData() override;
    void save2Cfg() override;
    void getUiData() override;
    void show2Ui() override;
    void callXml2Txt();
    void callNoXml2Txt();
    void callRename();
    void callSpliteDataset();
    void callReNoTag();

private slots:
    void on_btnStartAddData_clicked();
    void on_btnAddTag_clicked();
    void on_btnRemoveTag_clicked();
    void on_tableWidgetTag_cellDoubleClicked(int row, int column);

private:
    Ui::WidgetAddData *ui;
    ini_add_data m_data;

};

#endif // WIDGETADDDATA_H
