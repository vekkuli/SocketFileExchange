#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("vekkuli");
    QCoreApplication::setApplicationName("FileExchange");
    MainWindow w;
    w.show();

    return a.exec();
}
