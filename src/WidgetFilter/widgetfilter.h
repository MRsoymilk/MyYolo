#ifndef WIDGETFILTER_H
#define WIDGETFILTER_H

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

private:
    Ui::WidgetFilter *ui;

    // ConfigHandler interface
    void initFilter();

public:
    void getCfgData() override;
    void save2Cfg() override;
    void show2Ui() override;
    void getUiData() override;
};

#endif // WIDGETFILTER_H
