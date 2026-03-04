#include <QApplication>

#include "mainwindow.h"
#include "funcdef.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SETTING_INIT("config/Ttentacle.cfg");
    MainWindow w;
    w.show();
    return a.exec();
}
