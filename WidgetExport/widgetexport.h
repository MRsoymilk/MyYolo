#ifndef WIDGETEXPORT_H
#define WIDGETEXPORT_H

#include <QProcess>
#include <QWidget>

namespace Ui {
class WidgetExport;
}

class WidgetExport : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetExport(QWidget *parent = nullptr);
    ~WidgetExport();
    struct ini_export {
        QStringList list_type = {};
        QString pt_model = "";
        QString dir_output = "";
    };
private:
    void getCfgData();
    void save2Cfg();
    void show2Ui();
    void getUiData();
    void runScript(const QString &script_path, const QString &class_name, const QString &input_folder, const QString &output_folder);
private slots:
    void initExport();
    void on_btnStartExport_clicked();
    void onProcessOutput();
    void onProcessError();
    void onProcessFinished(int exitCode);

private:
    Ui::WidgetExport *ui;
    ini_export m_data;
};

#endif // WIDGETEXPORT_H
