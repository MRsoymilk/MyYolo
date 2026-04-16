#ifndef WIDGETDEPLOY_H
#define WIDGETDEPLOY_H

#include <QStandardItem>
#include <QWidget>
#include "httplib.h"

namespace Ui
{
class WidgetDeploy;
}

class WidgetDeploy : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetDeploy(QWidget *parent = nullptr);
    ~WidgetDeploy();

private slots:
    void on_btnDeploy_clicked();

private:
    void init();
    void startServer();
    void stopServer();
    void addRouter();
    void addAPI2Table(QString name, QString method, QString param);

private:
    Ui::WidgetDeploy *ui;
private:
    httplib::Server *m_server = nullptr;
    QThread *m_listenThread = nullptr;
    std::atomic_bool m_running{false};
    bool m_enableServer = false;
    QStandardItemModel *m_modelApiTable = nullptr;

};

#endif  // WIDGETDEPLOY_H
