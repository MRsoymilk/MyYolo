#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) { ui->setupUi(this); }

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_tBtnClangFormatLoad_clicked()
{

}

void MainWindow::on_tBtnCodeLoad_clicked()
{

}


void MainWindow::on_btnFormat_clicked()
{

}

