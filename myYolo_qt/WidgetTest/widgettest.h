#ifndef WIDGETTEST_H
#define WIDGETTEST_H

#include <QLineEdit>
#include <QWidget>

#include "../common/confighandler.h"
#include "onnxcheck.h"

namespace Ui
{
class WidgetTest;
}

class WidgetTest : public QWidget, public ConfigHandler
{
    Q_OBJECT

   public:
    explicit WidgetTest(QWidget *parent = nullptr);
    ~WidgetTest();
    struct ini_test
    {
        int model_height = 0;
        int model_width = 0;
        double threshold_nms = 0.0;
        double threshold_cfd = 0.0;
        QString model_onnx = "";
        QString model_pt = "";
        QString dir_input = "";
        QString dir_output = "";
    };

   private:
    void initTest();
    void testOnnx();
    void getUiData() override;
    void getCfgData() override;
    void save2Cfg() override;
    void show2Ui() override;
    void testPt();

   private slots:
    void on_btnStartTest_clicked();
    void on_sBoxModelWidth_editingFinished();
    void on_sBoxModelHeight_editingFinished();
    void on_btnAddItem_clicked();
    void on_btnRemoveItem_clicked();
    void on_tableWidgetClassEdit_cellDoubleClicked(int row, int column);
    void on_doubleSpinBoxThresholdCfd_editingFinished();
    void on_doubleSpinBoxThresholdNMS_editingFinished();

   private:
    Ui::WidgetTest *ui;
    ini_test m_data;
    OnnxCheck m_onnxCheck;
};

#endif  // WIDGETTEST_H
