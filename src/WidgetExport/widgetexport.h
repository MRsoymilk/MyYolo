#ifndef WIDGETEXPORT_H
#define WIDGETEXPORT_H

#include <QProcess>
#include <QWidget>
#include "../common/confighandler.h"

namespace Ui {
class WidgetExport;
}

class WidgetExport : public QWidget, public ConfigHandler
{
    Q_OBJECT

public:
    explicit WidgetExport(QWidget *parent = nullptr);
    ~WidgetExport();
    struct ini_export {
        QStringList list_type = {};
        QString type = "";
        QString pt_model = "";
        QString dir_output = "";
    };

private:
    void getCfgData() override;
    void save2Cfg() override;
    void show2Ui() override;
    void getUiData() override;
    void runScript(const QString &script_path);

private slots:
    void initExport();
    void on_btnStartExport_clicked();
    void onProcessOutput();
    void onProcessError();
    void onProcessFinished(int exitCode);
    void on_btnAddType_clicked();
    void on_comboBoxType_currentTextChanged(const QString &type);

private:
    Ui::WidgetExport *ui;
    ini_export m_data;
};

#endif // WIDGETEXPORT_H
