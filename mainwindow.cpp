#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "WidgetTest/widgettest.h"
#include "WidgetTrain/widgettrain.h"
#include "WidgetAddData/widgetadddata.h"

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
    connect(ui->tBtnProjectRoot, &QPushButton::clicked, this, &MainWindow::openFileDialog);
}

void MainWindow::addBasicWidget() {
    WidgetAddData *widgetAddData = new WidgetAddData;
    WidgetTrain *widgetTrain = new WidgetTrain;
    WidgetTest *widgetTest = new WidgetTest;
    ui->gLayoutAddData->addWidget(widgetAddData);
    ui->gLayoutTrain->addWidget(widgetTrain);
    ui->gLayoutTest->addWidget(widgetTest);
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

void MainWindow::openFileDialog() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "select dir", QString(),
                                                           QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!folderPath.isEmpty()) {
        ui->lEditProjectRoot->setText(folderPath);
        m_data.path = folderPath;
    }
}
