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

void WidgetTest::on_btnStartTest_clicked()
{

}
