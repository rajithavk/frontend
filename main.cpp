#include "bow.h"
#include "inventory.h"
#include "mainwindow.h"
#include "imagecapture.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <iostream>


void loadStyleSheet(){
    QFile data(":/sheet/style.qss");
    QString style;
    if(data.open(QFile::ReadOnly)){
        QTextStream styleIn(&data);
        style = styleIn.readAll();
        data.close();
        qApp->setStyleSheet(style);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //BOW w;
    //Imagegrab w;
    //MainWindow w;
    Inventory w(0,0);
    //ImageCapture w(0);

    loadStyleSheet();
    w.show();

    return a.exec();
}
