#include "widgettest.h"
#include "ui_widgettest.h"

#include <QFileDialog>

WidgetTest::WidgetTest(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetTest)
{
    ui->setupUi(this);
}

WidgetTest::~WidgetTest()
{
    delete ui;
}

void WidgetTest::initTest() {

}

void WidgetTest::on_tBtnModel_triggered(QAction *arg1)
{

}

#include <QDebug>

void WidgetTest::on_btnStartTest_clicked()
{

}


void WidgetTest::openFile(QLineEdit *edit) {
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", QString(),
                                                    "All Files (*);;Text Files (*.txt);;Image Files (*.png *.jpg);;");
    if (!filePath.isEmpty()) {
        edit->setText(filePath);
    }
}


void WidgetTest::openFolder(QLineEdit *edit) {
    QString folderPath = QFileDialog::getExistingDirectory(this, "select dir", QString(),
                                                           QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!folderPath.isEmpty()) {
        edit->setText(folderPath);
    }
}
