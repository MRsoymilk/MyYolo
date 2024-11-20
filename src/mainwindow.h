#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    struct ini_main {
        QString path = "";
    };
private:
    void addBasicWidget();
    void initMain();
private:
    Ui::MainWindow *ui;
    ini_main m_data;
};
#endif // MAINWINDOW_H
