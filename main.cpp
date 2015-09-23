/*
* CPSC 453 - Introduction to Computer Graphics
* Assignment 1
*
* Entry point for execution into the program
*/

#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
