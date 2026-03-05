#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tBtnClangFormatLoad_clicked();
    void on_tBtnCodeLoad_clicked();
    void on_btnFormat_clicked();

private:
    void init();

private:
    Ui::MainWindow *ui;
};
#endif  // MAINWINDOW_H
