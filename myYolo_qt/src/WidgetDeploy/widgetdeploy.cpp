#include "widgetdeploy.h"

#include <QThread>

#include "funcdef.h"
#include "keydef.h"
#include "my-yolo-inference.h"
#include "ui_widgetdeploy.h"

WidgetDeploy::WidgetDeploy(QWidget *parent) : QWidget(parent), ui(new Ui::WidgetDeploy)
{
    ui->setupUi(this);
    init();
}

WidgetDeploy::~WidgetDeploy() { delete ui; }

void WidgetDeploy::on_btnDeploy_clicked()
{
    m_enableServer = !m_enableServer;
    ui->btnDeploy->setChecked(m_enableServer);
    if (m_enableServer)
    {
        startServer();
    }
    else
    {
        stopServer();
    }
}

void WidgetDeploy::init()
{
    QString ip = SETTING_GET(CFG_GROUP_DEPLOY, CFG_DEPLOY_IP, "0.0.0.0");
    QString port = SETTING_GET(CFG_GROUP_DEPLOY, CFG_DEPLOY_PORT, "18000");
    QString model = SETTING_GET(CFG_GROUP_DEPLOY, CFG_DEPLOY_MODEL);
    ui->lineEditIP->setText(ip);
    ui->lineEditPort->setText(port);
    ui->lineEditModel->setText(model);

    m_modelApiTable = new QStandardItemModel(this);

    m_modelApiTable->setHorizontalHeaderLabels({"name", "type", "params"});

    ui->tableViewAPI->setModel(m_modelApiTable);
    ui->tableViewAPI->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewAPI->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewAPI->setEditTriggers(QAbstractItemView::NoEditTriggers);

    addAPI2Table("/info", "GET", "");
    addAPI2Table("/inference", "POST", "");

    REGISTER_FILE_BTN(ui->tBtnLoadModel, ui->lineEditModel);
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
    SETTING_SET(CFG_GROUP_DEPLOY, CFG_DEPLOY_IP, ui->lineEditIP->text());
    SETTING_SET(CFG_GROUP_DEPLOY, CFG_DEPLOY_PORT, ui->lineEditPort->text());
    SETTING_SET(CFG_GROUP_DEPLOY, CFG_DEPLOY_MODEL, ui->lineEditModel->text());

    if (!m_server)
    {
        m_server = new httplib::Server();
        m_server->new_task_queue = [] { return new httplib::ThreadPool(4); };
        addRouter();
    }

    if (m_running.load()) return;

    m_running.store(true);

    QString ip = ui->lineEditIP->text();
    int port = ui->lineEditPort->text().toInt();
    LOG_INFO("start http server on {}:{}", ip, port);

    QString model = ui->lineEditModel->text();
    bool status = MY_YOLO.loadModel(model.toStdString().c_str());
    LOG_INFO("load model: {}, status: {}", model, status);

    m_listenThread = QThread::create(
        [this, ip, port]()
        {
            m_server->listen(ip.toStdString(), port);
            LOG_INFO("http listen thread exited");
        });

    connect(m_listenThread, &QThread::finished, m_listenThread, &QObject::deleteLater, Qt::QueuedConnection);

    m_listenThread->start();
}

void WidgetDeploy::stopServer()
{
    if (!m_server || !m_running.load()) return;

    LOG_INFO("stop http server");

    m_running.store(false);

    m_server->stop();

    if (m_listenThread)
    {
        m_listenThread->wait();
        m_listenThread = nullptr;
    }
}

void WidgetDeploy::registerInfo()
{
    m_server->Get("/info",
                  [this](const httplib::Request &, httplib::Response &res)
                  {
                      if (!m_running.load())
                      {
                          res.status = 503;
                          return;
                      }

                      QJsonObject objInfo;
                      objInfo["status"] = "ok";
                      objInfo["interface_name"] = "my yolo";
                      objInfo["timestamp"] = TIMESTAMP();

                      char json_buffer[10480] = {0};
                      unsigned int json_buffer_len = 0;

                      MY_YOLO.getModelInfo(json_buffer, &json_buffer_len);

                      if (json_buffer_len > 0)
                      {
                          QByteArray arr(json_buffer, json_buffer_len);

                          QJsonParseError err;
                          QJsonDocument doc = QJsonDocument::fromJson(arr, &err);

                          if (err.error == QJsonParseError::NoError && doc.isObject())
                          {
                              objInfo["model_info"] = doc.object();
                          }
                          else
                          {
                              objInfo["model_info"] = QString::fromUtf8(arr);
                          }
                      }

                      QJsonDocument doc(objInfo);
                      res.set_content(doc.toJson(QJsonDocument::Compact).toStdString(), "application/json");
                  });
}

void WidgetDeploy::registerInference()
{
    m_server->Post("/inference",
                   [this](const httplib::Request &req, httplib::Response &res)
                   {
                       if (!m_running.load())
                       {
                           res.status = 503;
                           return;
                       }

                       QJsonObject result;

                       QJsonParseError err;
                       QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(req.body), &err);

                       if (err.error != QJsonParseError::NoError)
                       {
                           res.status = 400;
                           result["status"] = "json error";
                           res.set_content(TO_STR(result).toStdString(), "application/json");
                           return;
                       }

                       QJsonObject obj = doc.object();

                       QString name = obj["name"].toString();
                       QString type = obj["type"].toString();
                       QString data = obj["data"].toString();

                       // Base64 -> 图片bytes
                       QByteArray imgBytes = QByteArray::fromBase64(data.toUtf8());

                       if (imgBytes.isEmpty())
                       {
                           res.status = 400;
                           result["status"] = "image decode error";
                           res.set_content(TO_STR(result).toStdString(), "application/json");
                           return;
                       }

                       // 推理
                       char json_buffer[20480] = {0};
                       unsigned int json_buffer_len = 0;

                       bool ok = MY_YOLO.inference(imgBytes.data(), imgBytes.size(), json_buffer, &json_buffer_len);
                       qDebug() << json_buffer;

                       if (!ok)
                       {
                           res.status = 500;
                           result["status"] = "inference failed";
                           res.set_content(TO_STR(result).toStdString(), "application/json");
                           return;
                       }

                       // 解析推理结果
                       QJsonDocument inferDoc = QJsonDocument::fromJson(QByteArray(json_buffer, json_buffer_len));

                       result["status"] = "ok";
                       result["recv_name"] = name;
                       result["type"] = type;
                       result["timestamp"] = TIMESTAMP();
                       result["result"] = inferDoc.object();

                       res.set_content(TO_STR(result).toStdString(), "application/json");
                   });
}

void WidgetDeploy::addRouter()
{
    registerInfo();
    registerInference();
}
