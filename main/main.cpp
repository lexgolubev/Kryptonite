#include "mainwindow.h"
#include "def.h"
#include <QApplication>
#include <QDebug>

#ifndef TEST

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

#endif
