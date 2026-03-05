#include "mainwindow.h"

#include "./ui_mainwindow.h"
#include "funcdef.h"
#include "keydef.h"
#include <QDirIterator>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) { ui->setupUi(this); init(); }

MainWindow::~MainWindow() { delete ui; }

void MainWindow::init() {
    QString path_clang_format = SETTING_GET(CFG_GROUP_CODE_FMT, CFG_FMT_CLANG_FORMAT);
    QString path_code = SETTING_GET(CFG_GROUP_CODE_FMT, CFG_FMT_CODE);
    ui->lineEditClangFormat->setText(path_clang_format);
    ui->lineEditCode->setText(path_code);
}

void MainWindow::on_tBtnClangFormatLoad_clicked()
{
    OPEN_FILE(ui->lineEditClangFormat);
    SETTING_SET(CFG_GROUP_CODE_FMT, CFG_FMT_CLANG_FORMAT, ui->lineEditClangFormat->text());
}

void MainWindow::on_tBtnCodeLoad_clicked()
{
    OPEN_FOLDER(ui->lineEditCode);
    SETTING_SET(CFG_GROUP_CODE_FMT, CFG_FMT_CODE, ui->lineEditCode->text());
}

void MainWindow::on_btnFormat_clicked()
{
    QString clangFormat = ui->lineEditClangFormat->text();
    QString codePath = ui->lineEditCode->text();

    if (clangFormat.isEmpty() || codePath.isEmpty())
        return;

    QStringList filters;
    filters << "*.cpp" << "*.h";

    QDirIterator it(
        codePath,
        filters,
        QDir::Files,
        QDirIterator::Subdirectories
        );

    int count = 0;

    while (it.hasNext())
    {
        QString file = it.next();
        ++count;
        QProcess process;
        ui->textBrowserCodeLog->append(QString("format file: %1").arg(file));
        process.start(
            "clang-format",
            QStringList()
                << "-i"
                << QString("-style=file:%1").arg(clangFormat)
                << file
            );
        process.waitForFinished();
    }
    ui->textBrowserCodeLog->append(QString("finish with %1 file.").arg(count));
}
