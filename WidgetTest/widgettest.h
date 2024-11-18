#ifndef WIDGETTEST_H
#define WIDGETTEST_H

#include <QLineEdit>
#include <QWidget>
#include <QProcess>
#include "onnxcheck.h"

namespace Ui {
class WidgetTest;
}

class WidgetTest : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTest(QWidget *parent = nullptr);
    ~WidgetTest();
    struct ini_test {
        int model_height = 0;
        int model_width = 0;
        QString model = "";
        QString dir_input = "";
        QString dir_output = "";
    };
private:
    void initTest();
    void getUiData();
    void testOnnx();
    void getCfgData();
    void save2Cfg();
    void show2Ui();
private slots:
    void on_btnStartTest_clicked();
    void on_sBoxModelWidth_editingFinished();
    void on_sBoxModelHeight_editingFinished();
private:
    Ui::WidgetTest *ui;
    ini_test m_data;
    OnnxCheck m_check;
};

#endif // WIDGETTEST_H
