#include "mainwindow.h"
#include <QApplication>
#define _CRT_SECURE_NO_WARNINGS
#include "iostream"
#include <cstdio>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
