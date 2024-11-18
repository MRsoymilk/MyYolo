#ifndef WIDGETADDDATA_H
#define WIDGETADDDATA_H

#include <QWidget>
#include <QLineEdit>
#include <QProcess>

namespace Ui {
class WidgetAddData;
}

class WidgetAddData : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetAddData(QWidget *parent = nullptr);
    ~WidgetAddData();
    struct ini_add_data {
        QString dir_input = "";
        QString dir_output = "";
        QString script = "";
        QString tag = "";
        QString dir_todo = "";
        int train = 0;
        int valid = 0;
        int test = 0;
    };
private:
    Ui::WidgetAddData *ui;
    void initAddData();
    void getCfgData();
    void save2Cfg();
    void getUiData();
    void show2Ui();
    void runScript(const QString &script_path, const QString &class_name, const QString &input_folder, const QString &output_folder);
private slots:
    void on_btnStartAddData_clicked();
    void on_btnHandleNoGap_clicked();
    void onProcessOutput();
    void onProcessError();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
private:
    ini_add_data m_data;
};

#endif // WIDGETADDDATA_H
