#ifndef WIDGETMAINSETTING_H
#define WIDGETMAINSETTING_H

#include <QWidget>
#include "../../include/datadef.h"
#include "../../common/confighandler.h"

namespace Ui {
class WidgetMainSetting;
}

class WidgetMainSetting : public QWidget, public ConfigHandler
{
    Q_OBJECT

public:
    explicit WidgetMainSetting(QWidget *parent = nullptr);
    ~WidgetMainSetting();
    struct ini_main_setting
    {
        QString project_dir = "";
        QString cmd_python = "";
        QString cmd_pip = "";
    };

private:
    MSG_RE checkPython();
    void initMainSetting();
    MSG_RE checkVenv();
    MSG_RE checkScript();
    MSG_RE checkTools();
    MSG_RE checkYolo5();
    void getCfgData() override;
    void save2Cfg() override;
    void show2Ui() override;
    void getUiData() override;

private slots:
    void on_btnBasicCheck_clicked();

private:
    Ui::WidgetMainSetting *ui;
    ini_main_setting m_data;
};

#endif // WIDGETMAINSETTING_H
