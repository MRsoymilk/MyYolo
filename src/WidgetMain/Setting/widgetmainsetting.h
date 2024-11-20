#ifndef WIDGETMAINSETTING_H
#define WIDGETMAINSETTING_H

#include <QWidget>

namespace Ui {
class WidgetMainSetting;
}

class WidgetMainSetting : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMainSetting(QWidget *parent = nullptr);
    ~WidgetMainSetting();

private:
    Ui::WidgetMainSetting *ui;
    void checkBasicDir();
    void checkPython();
};

#endif // WIDGETMAINSETTING_H
