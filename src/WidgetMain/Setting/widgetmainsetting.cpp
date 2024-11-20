#include "widgetmainsetting.h"
#include "ui_widgetmainsetting.h"


#include "../../include/funcdef.h"
#include "../../include/keydef.h"
#include "../../util/mysetting.h"

#include <QProcess>
#include "../../include/global.h"
#include "../../util/mydir.h"
#include "../../util/mylog.h"

WidgetMainSetting::WidgetMainSetting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetMainSetting)
{
    ui->setupUi(this);
    initMainSetting();
}

void WidgetMainSetting::initMainSetting()
{
    OPEN_FOLDER_BTN(ui->tBtnProjectRoot, ui->lEditProjectRoot);
}

WidgetMainSetting::~WidgetMainSetting()
{
    delete ui;
}

MSG_RE WidgetMainSetting::checkBasicDir()
{
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

MSG_RE WidgetMainSetting::checkVenv() {}

MSG_RE WidgetMainSetting::checkPython()
{
    MSG_RE re;
    QProcess process;

    // python
    process.start("python", QStringList() << "--version");
    if (!process.waitForFinished()) {
        re.status = false;
        re.msg = "Failed to start python process: " + process.readAllStandardError();
        return re;
    }
    if (process.exitCode() == 0) {
        re.status = true;
        re.msg = process.readAllStandardOutput().trimmed();
        return re;
    }

    // python3
    process.start("python3", QStringList() << "--version");
    if (!process.waitForFinished()) {
        re.status = false;
        re.msg = "Failed to start python3 process: " + process.readAllStandardError();
        return re;
    }
    if (process.exitCode() == 0) {
        re.status = true;
        re.msg = process.readAllStandardOutput().trimmed();
        return re;
    }

    re.status = false;
    re.msg = "Python or Python3 is not installed or not found.";
    return re;
    //     // Step 2: Create a Python virtual environment if not already created
    //     QString venvPath = "project/yolov5-master/venv";
    //     QDir venvDir(venvPath);
    //     if (!venvDir.exists()) {
    //         TXT_INFO(QString("Creating virtual environment in %1").arg(venvPath));
    //         // Create the virtual environment
    //         QProcess venvProcess;
    //         venvProcess.start(pythonCommand, QStringList() << "-m" << "venv" << venvPath);
    //         venvProcess.waitForFinished();
    //         if (venvProcess.exitCode() != 0) {
    //             TXT_WARN("Failed to create virtual environment.");
    //             return; // Exit if virtual environment creation fails
    //         }
    //     } else {
    //         TXT_INFO("Virtual environment already exists.");
    //     }

    //     // Step 3: Set the Python executable from the virtual environment
    //     QString pythonExecutable;
    // #ifdef Q_OS_WIN
    //     pythonExecutable = venvPath + "/Scripts/python.exe"; // Windows path
    // #endif
    // #ifdef Q_OS_UNIX
    //     pythonExecutable = venvPath + "/bin/python"; // Linux/Mac path
    // #endif

    //     // Check if the virtual environment's python executable exists
    //     QFileInfo pythonFileInfo(pythonExecutable);
    //     if (pythonFileInfo.exists() && pythonFileInfo.isExecutable()) {
    //         qDebug() << "Using Python executable from virtual environment:" << pythonExecutable;
    //     } else {
    //         qWarning() << "Virtual environment's Python executable not found!";
    //     }
}

void WidgetMainSetting::on_btnBasicCheck_clicked()
{
    MSG_RE re;
    re = checkPython();
    if (!re.status) {
        TXT_WARN(QString("Check Python error: %1").arg(re.msg));
        return;
    } else {
        TXT_INFO(QString("Python Detect: %1").arg(re.msg));
    }
    re = checkVenv();
}

