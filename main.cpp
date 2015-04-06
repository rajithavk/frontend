//#include "bow.h"
//#include "inventory.h"
#include "mainwindow.h"
//#include "imagecapture.h"
#include <QApplication>
#include <QPixmap>
#include <QLabel>
#include <cstdio>
#include <map>
#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include "welcome.h"







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
    //Inventory w(1,0);
    //ImageCapture w(0);






    welcome w;

    loadStyleSheet();
    w.show();

    return a.exec();
}
