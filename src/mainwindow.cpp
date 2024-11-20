#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "WidgetMain/Info/widgetmaininfo.h"
#include "WidgetMain/Setting/widgetmainsetting.h"
#include "WidgetTest/widgettest.h"
#include "WidgetTrain/widgettrain.h"
#include "WidgetAddData/widgetadddata.h"
#include "WidgetExport/widgetexport.h"

#include <QFileDialog>

#include "include/funcdef.h"
#include "include/keydef.h"
#include "util/mysetting.h"

#include "util/mylog.h"
#include "util/mydir.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMain();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMain() {
    addBasicWidget();
}

void MainWindow::addBasicWidget() {
    WidgetMainInfo *widgetMainInfo = new WidgetMainInfo();
    WidgetMainSetting *widgetMainSetting = new WidgetMainSetting();
    WidgetAddData *widgetAddData = new WidgetAddData();
    WidgetTrain *widgetTrain = new WidgetTrain();
    WidgetTest *widgetTest = new WidgetTest();
    WidgetExport *widgetExport = new WidgetExport();
    ui->gLayoutAddData->addWidget(widgetAddData);
    ui->gLayoutTrain->addWidget(widgetTrain);
    ui->gLayoutTest->addWidget(widgetTest);
    ui->gLayoutExport->addWidget(widgetExport);
    ui->gLayInfo->addWidget(widgetMainInfo);
    ui->gLaySetting->addWidget(widgetMainSetting);
}

