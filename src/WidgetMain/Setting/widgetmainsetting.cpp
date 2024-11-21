#include "widgetmainsetting.h"
#include "ui_widgetmainsetting.h"


#include "../../include/funcdef.h"
#include "../../include/keydef.h"
#include "../../util/mysetting.h"

#include <QProcess>
#include "../../include/global.h"
#include "../../util/mydir.h"
#include "../../util/mylog.h"
#include "../../util/mysetting.h"

WidgetMainSetting::WidgetMainSetting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetMainSetting)
{
    ui->setupUi(this);
    initMainSetting();
}

void WidgetMainSetting::initMainSetting()
{
    getCfgData();
    show2Ui();
    OPEN_FOLDER_BTN(ui->tBtnProjectRoot, ui->lEditProjectRoot);
    on_btnBasicCheck_clicked();
}

WidgetMainSetting::~WidgetMainSetting()
{
    delete ui;
}

MSG_RE WidgetMainSetting::checkVenv() {
    MSG_RE re;
    QString path = m_data.project_dir + "/venv";
    QProcess cmd;
    QDir venv(path);
    if (!venv.exists()) {
        TXT_INFO(QString("Creating virtual environment in %1").arg(path));
        cmd.start(m_data.cmd_python, QStringList() << "-m" << "venv" << path);
        cmd.waitForFinished();
        if (cmd.exitCode() != 0) {
            TXT_WARN("Failed to create virtual environment.");
            re.status = false;
            re.code = 1;
            return re;
        }
    }
    else {
        TXT_INFO("Virtual environment already exists.");
    }
#ifdef Q_OS_WIN
    GLOBAL.PYTHON = path + "/Scripts/python.exe"; // Windows
    GLOBAL.PIP = path + "/Scripts/pip.exe";
#endif
#ifdef Q_OS_UNIX
    GLOBAL.PYTHON = path + "/bin/python"; // Linux/Mac
    GLOBAL.PIP = path + "/bin/pip";
#endif
    cmd.start(m_data.cmd_pip, QStringList() << "config" << "set" << "global.index-url" << "https://mirrors.tuna.tsinghua.edu.cn/pypi/web/simple");
    cmd.waitForFinished();
    if (cmd.exitCode() != 0) {
        TXT_WARN("Failed to set global pip mirrors.");
        re.status = false;
        re.code = 2;
        return re;
    }
    re.status = true;
    return re;
}

MSG_RE WidgetMainSetting::checkScript()
{
    MSG_RE re;
    GLOBAL.SCRIPT_XML2TXT = QString("%1%2").arg(m_data.project_dir, "/script/xml2txt.py");
    if(!QFile::exists(GLOBAL.SCRIPT_XML2TXT)) {
        re.status = false;
        re.msg = QString("no script xml2txt.py found: %1").arg(GLOBAL.SCRIPT_XML2TXT);
        return re;
    }
    GLOBAL.SCRIPT_NO_XML2TXT = QString("%1%2").arg(m_data.project_dir, "/script/no_xml2txt.py");
    if(!QFile::exists(GLOBAL.SCRIPT_NO_XML2TXT)) {
        re.status = false;
        re.msg = QString("no script no_xml2txt.py found: %1").arg(GLOBAL.SCRIPT_NO_XML2TXT);
        return re;
    }
    GLOBAL.SCRIPT_SPLIT_DATASET = QString("%1%2").arg(m_data.project_dir, "/script/split_dataset.py");
    if(!QFile::exists(GLOBAL.SCRIPT_SPLIT_DATASET)) {
        re.status = false;
        re.msg = QString("no script split_dataset.py found: %1").arg(GLOBAL.SCRIPT_SPLIT_DATASET);
        return re;
    }
    GLOBAL.SCRIPT_RENAME = QString("%1%2").arg(m_data.project_dir, "/script/rename.py");
    if(!QFile::exists(GLOBAL.SCRIPT_RENAME)) {
        re.status = false;
        re.msg = QString("no script rename.py found: %1").arg(GLOBAL.SCRIPT_RENAME);
        return re;
    }
    GLOBAL.SCRIPT_RE_NOTAG = QString("%1%2").arg(m_data.project_dir, "/script/re_notag.py");
    if(!QFile::exists(GLOBAL.SCRIPT_RE_NOTAG)) {
        re.status = false;
        re.msg = QString("no script re_notag.py found: %1").arg(GLOBAL.SCRIPT_RE_NOTAG);
        return re;
    }
    re.status = true;
    return re;
}

MSG_RE WidgetMainSetting::checkTools()
{
    MSG_RE re;
#ifdef Q_OS_WIN
    GLOBAL.EXE_LABELIMG = QString("%1/%2").arg(m_data.project_dir, "/tools/windows/labelImg/labelImg.exe");
#endif
#ifdef Q_OS_UNIX
#endif
    return re;
}

MSG_RE WidgetMainSetting::checkYolo5()
{
    MSG_RE re;
    return re;
}

void WidgetMainSetting::getCfgData()
{
    m_data.project_dir = MY_SETTING.getValue(CFG_GROUP_MAIN, CFG_MAIN_PROJECT_DIR);
}

void WidgetMainSetting::save2Cfg()
{
    MY_SETTING.setValue(CFG_GROUP_MAIN, CFG_MAIN_PROJECT_DIR, m_data.project_dir);
}

void WidgetMainSetting::show2Ui()
{
    ui->lEditProjectRoot->setText(m_data.project_dir);
}

void WidgetMainSetting::getUiData()
{
    m_data.project_dir = ui->lEditProjectRoot->text();
}

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
        re.code = 1;
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
        re.code = 2;
        return re;
    }

    re.status = false;
    re.msg = "Python or Python3 is not installed or not found.";
    return re;
}

void WidgetMainSetting::on_btnBasicCheck_clicked()
{
    getUiData();
    save2Cfg();
    MSG_RE re;
    getUiData();
    re = checkPython();
    if (!re.status) {
        TXT_WARN(QString("Check Python error: %1").arg(re.msg));
        return;
    } else {
        TXT_INFO(QString("Python Detect: %1").arg(re.msg));
        if(re.code == 1) {
            m_data.cmd_python = "python";
        }
        if(re.code == 2) {
            m_data.cmd_python = "python3";
        }
    }
    if(m_data.project_dir.isEmpty()) {
        m_data.project_dir = "./project";
        TXT_WARN(QString("Use default project root: ./project"));
    }
    else {
        m_data.project_dir = MyDir::GetAbsolutePath(m_data.project_dir);
        TXT_INFO(QString("Use project root: %1").arg(m_data.project_dir));
    }
    re = checkVenv();
    if(!re.status) {
        TXT_WARN(QString("Check python venv error: %1").arg(re.msg));
        return;
    }
    else {
        TXT_INFO(QString("Venv Detect: %1").arg(re.msg));
    }
    re = checkScript();
    re = checkTools();
    re = checkYolo5();
}
