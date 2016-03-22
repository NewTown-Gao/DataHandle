#include <QCoreApplication>
#include <QThread>
#include "mainenter.h"
#include "acesstask.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    QCoreApplication::addLibraryPath("/home/authon_package/plugins/sqldrivers/");
//    QCoreApplication::addLibraryPath("/home/authon_package/");

    QString strLibPath(QDir::toNativeSeparators(QCoreApplication::applicationDirPath())
                       + QDir::separator() + "plugins");
    a.addLibraryPath(strLibPath);

    MainEnter enter;

    return a.exec();
}
