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
    QString root = MY_SETTING.getValue(CFG_GROUP_MAIN, CFG_MAIN_PROJECT_DIR, MyDir::GetAbsolutePath("./project"));
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

void MainWindow::checkPython() {
    QString pythonCommand;
    QString pythonPath;

    // Step 1: Check if Python is installed
    QProcess process;
    process.start("python --version");
    process.waitForFinished();
    if (process.exitCode() != 0) {
        // Python not found, try python3
        process.start("python3 --version");
        process.waitForFinished();
        if (process.exitCode() != 0) {
            qWarning() << "Python not found. Installing virtual environment will fail.";
            return; // Exit if no Python is found
        } else {
            pythonCommand = "python3";
            pythonPath = "python3";
        }
    } else {
        pythonCommand = "python";
        pythonPath = "python";
    }

    // Step 2: Create a Python virtual environment if not already created
    QString venvPath = "project/yolov5-master/venv";
    QDir venvDir(venvPath);
    if (!venvDir.exists()) {
        qDebug() << "Creating virtual environment in" << venvPath;

        // Create the virtual environment
        QProcess venvProcess;
        venvProcess.start(pythonCommand, QStringList() << "-m" << "venv" << venvPath);
        venvProcess.waitForFinished();
        if (venvProcess.exitCode() != 0) {
            qWarning() << "Failed to create virtual environment.";
            return; // Exit if virtual environment creation fails
        }
    } else {
        qDebug() << "Virtual environment already exists.";
    }

    // Step 3: Set the Python executable from the virtual environment
    QString pythonExecutable;
#ifdef Q_OS_WIN
    pythonExecutable = venvPath + "/Scripts/python.exe"; // Windows path
#endif
#ifdef Q_OS_UNIX
    pythonExecutable = venvPath + "/bin/python"; // Linux/Mac path
#endif

    // Check if the virtual environment's python executable exists
    QFileInfo pythonFileInfo(pythonExecutable);
    if (pythonFileInfo.exists() && pythonFileInfo.isExecutable()) {
        qDebug() << "Using Python executable from virtual environment:" << pythonExecutable;
    } else {
        qWarning() << "Virtual environment's Python executable not found!";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBasicCheck_clicked()
{
    checkBasicDir();
    checkPython();
}
