#ifndef WIDGETTRAIN_H
#define WIDGETTRAIN_H

#include <QWidget>
#include <QLineEdit>
#include "../common/confighandler.h"

namespace Ui {
class WidgetTrain;
}

class WidgetTrain : public QWidget, public ConfigHandler
{
    Q_OBJECT

public:
    explicit WidgetTrain(QWidget *parent = nullptr);
    ~WidgetTrain();

private slots:
    void on_btnStartTrain_clicked();

private:
    Ui::WidgetTrain *ui;
    void initTrain();

    struct ini_train {
        QStringList list_model = {};
        QString weights = "";
        QString data = "";
        QString output = "";
        QString hyp = "";
        int epoch = 0;
        int batch_size = 0;
        int img_size = 0;
    };
    ini_train m_data;
    void getCfgData() override;
    void getUiData() override;
    void show2Ui() override;
    void save2Cfg() override;
    void runScript(const QString &base_dir, const QString &script);
};

#endif // WIDGETTRAIN_H
