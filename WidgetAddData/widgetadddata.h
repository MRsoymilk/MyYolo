#ifndef WIDGETADDDATA_H
#define WIDGETADDDATA_H

#include <QWidget>
#include <QLineEdit>

namespace Ui {
class WidgetAddData;
}

class WidgetAddData : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetAddData(QWidget *parent = nullptr);
    ~WidgetAddData();

private slots:
    void on_tBtnInput_triggered(QAction *arg1);
    void on_tBtnOutput_triggered(QAction *arg1);
    void on_btnStartAddData_clicked();
    void openFolder(QLineEdit *edit);
    void openFile(QLineEdit *edit);

private:
    Ui::WidgetAddData *ui;
    void initAddData();
    void getCfgData();
    void save2Cfg();

private:
    struct ini_add_data {
        QString data_input = "";
        QString data_output = "";
        int train = 0;
        int valid = 0;
        int test = 0;
    };
    ini_add_data m_data;
    void getUiData();
    void show2Ui();
};

#endif // WIDGETADDDATA_H
