#ifndef WIDGETADDDATA_H
#define WIDGETADDDATA_H

#include <QWidget>
#include <QLineEdit>
#include <QProcess>
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
    Ui::WidgetAddData *ui;
    void initAddData();
    void getCfgData() override;
    void save2Cfg() override;
    void getUiData() override;
    void show2Ui() override;
    void runScript(const QString &script_path, const QString &class_name, const QString &input_folder, const QString &output_folder);
private slots:
    void on_btnStartAddData_clicked();
    // void on_btnHandleNoGap_clicked();
    void onProcessOutput();
    void onProcessError();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void on_btnAddTag_clicked();

    void on_btnRemoveTag_clicked();

    void on_tableWidgetTag_cellDoubleClicked(int row, int column);

private:
    ini_add_data m_data;
    void runScript(const QStringList &params);
    QProcess* m_process;
    void callXml2Txt();
    void callNoXml2Txt();
    void callRename();
    void callSpliteDataset();
    void callReNoTag();
};

#endif // WIDGETADDDATA_H
