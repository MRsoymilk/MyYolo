#include "widgetdeploy.h"

#include "ui_widgetdeploy.h"
#include "funcdef.h"
#include "keydef.h"

#include <QThread>

WidgetDeploy::WidgetDeploy(QWidget *parent) : QWidget(parent), ui(new Ui::WidgetDeploy) { ui->setupUi(this);
    init();
}

WidgetDeploy::~WidgetDeploy() { delete ui; }

void WidgetDeploy::on_btnDeploy_clicked()
{
    m_enableServer = !m_enableServer;
    ui->btnDeploy->setChecked(m_enableServer);
    if(m_enableServer) {
        startServer();
    }
    else {
        stopServer();
    }
}

void WidgetDeploy::init()
{
    QString ip = SETTING_GET(CFG_GROUP_DEPLOY, CFG_DEPLOY_IP, "0.0.0.0");
    QString port = SETTING_GET(CFG_GROUP_DEPLOY, CFG_DEPLOY_PORT, "18000");
    ui->lineEditIP->setText(ip);
    ui->lineEditPort->setText(port);

    m_modelApiTable = new QStandardItemModel(this);

    m_modelApiTable->setHorizontalHeaderLabels({
        "name",
        "type",
        "params"
    });

    ui->tableViewAPI->setModel(m_modelApiTable);
    ui->tableViewAPI->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewAPI->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewAPI->setEditTriggers(QAbstractItemView::NoEditTriggers);

    addAPI2Table("/info", "GET", "");
}

void WidgetDeploy::addAPI2Table(QString name, QString method, QString param)
{
    QList<QStandardItem *> row;

    row << new QStandardItem(name);
    row << new QStandardItem(method);
    row << new QStandardItem(param);

    m_modelApiTable->appendRow(row);
}

void WidgetDeploy::startServer()
{
    if (!m_server) {
        m_server = new httplib::Server();
        m_server->new_task_queue = [] { return new httplib::ThreadPool(4); };
        addRouter();
    }

    if (m_running.load())
        return;

    m_running.store(true);

    int port = ui->lineEditPort->text().toInt();
    LOG_INFO("start http server on port {}", port);

    m_listenThread = QThread::create([this, port]() {
        m_server->listen("0.0.0.0", port);
        LOG_INFO("http listen thread exited");
    });

    connect(m_listenThread,
            &QThread::finished,
            m_listenThread,
            &QObject::deleteLater,
            Qt::QueuedConnection);

    m_listenThread->start();
}

void WidgetDeploy::stopServer()
{
    if (!m_server || !m_running.load())
        return;

    LOG_INFO("stop http server");

    m_running.store(false);

    m_server->stop();

    if (m_listenThread) {
        m_listenThread->wait();
        m_listenThread = nullptr;
    }
}

void WidgetDeploy::addRouter()
{
    m_server->Get("/info", [this](const httplib::Request &, httplib::Response &res) {
        if (!m_running.load()) {
            res.status = 503;
            return;
        }
        QJsonObject objInfo;
        objInfo["status"] = "ok";
        objInfo["interface_name"] = "my yolo";
        objInfo["timestamp"] = TIMESTAMP();
        res.set_content(TO_STR(objInfo).toStdString(), "application/json");
    });
}
