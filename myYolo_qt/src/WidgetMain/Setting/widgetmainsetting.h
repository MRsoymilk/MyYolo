#ifndef WIDGETMAINSETTING_H
#define WIDGETMAINSETTING_H

#include <QWidget>

#include "../../include/datadef.h"

namespace Ui
{
class WidgetMainSetting;
}

class WidgetMainSetting : public QWidget
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
        int use_venv = 1;
    };

private:
    MSG_RE checkPython();
    void initMainSetting();
    MSG_RE checkVenv(const bool &use);
    MSG_RE checkScript();
    MSG_RE checkTools();
    MSG_RE checkYolo5();

private slots:
    void on_btnBasicCheck_clicked();
    void on_checkBoxUseVenv_checkStateChanged(const Qt::CheckState &arg1);

private:
    Ui::WidgetMainSetting *ui;
    ini_main_setting m_data;
};

#endif  // WIDGETMAINSETTING_H
