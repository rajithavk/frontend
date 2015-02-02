#include "bow.h"
#include "inventory.h"
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

    inventory w;

    loadStyleSheet();
    w.show();

    return a.exec();
}
