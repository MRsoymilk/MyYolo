#include <QApplication>

#include "mainwindow.h"
#include "funcdef.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SETTING_INIT("config/myYolo.cfg");
    a.setWindowIcon(QIcon(":/res/app.png"));
    MainWindow w;
    w.show();
    return a.exec();
}
