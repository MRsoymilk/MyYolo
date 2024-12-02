#ifndef WIDGETLABEL_H
#define WIDGETLABEL_H

#include <QProcess>
#include <QWidget>
#include "../common/confighandler.h"

namespace Ui {
class WidgetLabel;
}

class WidgetLabel : public QWidget, public ConfigHandler
{
    Q_OBJECT

public:
    explicit WidgetLabel(QWidget *parent = nullptr);
    ~WidgetLabel();
    struct ini_label
    {
        QString path_labelimg;
    };
    void getCfgData() override;
    void save2Cfg() override;
    void show2Ui() override;
    void getUiData() override;

private:
    void initLabel();

private slots:
    void on_btnStartLabel_clicked();
private:
    Ui::WidgetLabel *ui;
    ini_label m_data;
};

#endif // WIDGETLABEL_H
