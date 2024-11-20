#ifndef WIDGETMAINSETTING_H
#define WIDGETMAINSETTING_H

#include <QWidget>
#include "../../include/datadef.h"

namespace Ui {
class WidgetMainSetting;
}

class WidgetMainSetting : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMainSetting(QWidget *parent = nullptr);
    ~WidgetMainSetting();
private slots:
    void on_btnBasicCheck_clicked();

private:
    Ui::WidgetMainSetting *ui;
    MSG_RE checkBasicDir();
    MSG_RE checkPython();
    void initMainSetting();
    MSG_RE checkVenv();
};

#endif // WIDGETMAINSETTING_H
