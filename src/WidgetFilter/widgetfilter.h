#ifndef WIDGETFILTER_H
#define WIDGETFILTER_H

#include <QProcess>
#include <QWidget>
#include "../common/confighandler.h"

namespace Ui {
class WidgetFilter;
}

class WidgetFilter : public QWidget, public ConfigHandler
{
    Q_OBJECT

public:
    explicit WidgetFilter(QWidget *parent = nullptr);
    ~WidgetFilter();
    struct ini_filter
    {
        QString dir_input = "";
        QString dir_output = "";
        int is_ssim;
        double threshold_ssim = 0.9;
        int batch_ssim = 10;
    };
    void getCfgData() override;
    void save2Cfg() override;
    void show2Ui() override;
    void getUiData() override;

private:
    void initFilter();
    void callMoveSimilarImgs();
    void runScript(const QStringList &arguments);

private slots:
    void on_btnStartFilter_clicked();
    void onProcessOutput();
    void onProcessError();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    Ui::WidgetFilter *ui;
    ini_filter m_data;
    QProcess *m_process;
};

#endif // WIDGETFILTER_H
