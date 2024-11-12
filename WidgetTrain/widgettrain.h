#ifndef WIDGETTRAIN_H
#define WIDGETTRAIN_H

#include <QWidget>
#include <QLineEdit>

namespace Ui {
class WidgetTrain;
}

class WidgetTrain : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTrain(QWidget *parent = nullptr);
    ~WidgetTrain();

private slots:
    void on_tBtnWeights_triggered(QAction *arg1);

    void on_tBtnEditData_triggered(QAction *arg1);

    void on_tBtnOutput_triggered(QAction *arg1);

    void on_btnStartTrain_clicked();

    void openFolder(QLineEdit *edit);
    void openFile(QLineEdit *edit);
private:
    Ui::WidgetTrain *ui;
    void initTrain();

    struct ini_train {
        QString weights = "";
        QString data = "";
        QString output = "";
        int epoch = 0;
        int batch_size = 0;
        double l_rate = 0.0;
        int img_size = 0;
    };
    ini_train m_data;
    void getCfgData();
    void getUiData();
    void show2Ui();
    void save2Cfg();
};

#endif // WIDGETTRAIN_H
