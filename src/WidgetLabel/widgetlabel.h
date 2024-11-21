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
    struct ini_data {
        QString path_labelimg;
    };

private:
    Ui::WidgetLabel *ui;

    // ConfigHandler interface
    void initLabel();
    ini_data m_data;
private slots:
    void on_btnStartLabel_clicked();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

    void onReadyReadStandardError();
    void onReadyReadStandardOutput();
public:
    void getCfgData() override;
    void save2Cfg() override;
    void show2Ui() override;
    void getUiData() override;
    QProcess *m_process;
};

#endif // WIDGETLABEL_H
