#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "WidgetTest/widgettest.h"
#include "WidgetTrain/widgettrain.h"
#include "WidgetAddData/widgetadddata.h"
#include "WidgetExport/widgetexport.h"

#include <QFileDialog>

#include "include/keydef.h"
#include "util/mysetting.h"

#include "util/mylog.h"
#include "util/mydir.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    addBasicWidget();
    OPEN_FOLDER_BTN(ui->tBtnProjectRoot, ui->lEditProjectRoot);
}

void MainWindow::addBasicWidget() {
    WidgetAddData *widgetAddData = new WidgetAddData();
    WidgetTrain *widgetTrain = new WidgetTrain();
    WidgetTest *widgetTest = new WidgetTest();
    WidgetExport *widgetExport = new WidgetExport();
    ui->gLayoutAddData->addWidget(widgetAddData);
    ui->gLayoutTrain->addWidget(widgetTrain);
    ui->gLayoutTest->addWidget(widgetTest);
    ui->gLayoutExport->addWidget(widgetExport);
}

void MainWindow::checkBasicDir() {
    QString root = MY_SETTING.getValue(CFG_GROUP_MAIN, CFG_MAIN_PROJECT_DIR);
    // check dir test
    CHECK_AND_CREATE_DIR(root, DIR_TEST);
    // check dir model
    CHECK_AND_CREATE_DIR(root, DIR_MODEL);
    // check dir tool
    CHECK_AND_CREATE_DIR(root, DIR_TOOL);
    // check dir script
    CHECK_AND_CREATE_DIR(root, DIR_SCRIPT);
    // check dir yolov5
    CHECK_AND_CREATE_DIR(root, DIR_YOLOV5);
    // check dir data
    CHECK_AND_CREATE_DIR(root, DIR_DATA);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBasicCheck_clicked()
{
    checkBasicDir();
}
