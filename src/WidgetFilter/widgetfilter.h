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
    };

private:
    Ui::WidgetFilter *ui;

    // ConfigHandler interface
    void initFilter();
    ini_filter m_data;

    void callMoveSimilarImgs();

    void runScript(const QStringList &arguments);

public:
    void getCfgData() override;
    void save2Cfg() override;
    void show2Ui() override;
    void getUiData() override;
    QProcess *m_process;
private slots:
    void on_btnStartFilter_clicked();
};

#endif // WIDGETFILTER_H
