#ifndef WIDGETTRAIN_H
#define WIDGETTRAIN_H

#include <QWidget>
#include <QLineEdit>
#include <QProcess>
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
    struct ini_train {
        QStringList list_model = {};
        QString weights = "";
        QString cfg = "";
        QString hyp = "";
        QString data = "";
        QString project = "";
        QString name = "";
        int epochs = 0;
        int batch_size = 0;
        int img_size = 0;
    };

private:
    void getCfgData() override;
    void getUiData() override;
    void show2Ui() override;
    void save2Cfg() override;
    void initTrain();
    void callTrain();
    void runScript(const QStringList &arguments);
    void stopScript();

private slots:
    void on_btnStartTrain_clicked();
    void onProcessOutput();
    void onProcessError();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

    void on_btnCancel_clicked();

    void on_tBtnName_clicked();

private:
    Ui::WidgetTrain *ui;
    ini_train m_data;
    QProcess *m_process;
};

#endif // WIDGETTRAIN_H
