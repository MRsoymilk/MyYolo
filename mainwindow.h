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

private slots:
    void on_btnBasicCheck_clicked();

    void openFileDialog();
private:
    Ui::MainWindow *ui;
    void addBasicWidget();
    void checkBasicDir();

private:
    struct ini_main {
        QString path = "";
    };

    ini_main m_data;
};
#endif // MAINWINDOW_H
