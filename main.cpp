#include "bow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <iostream>


void loadStyleSheet(){
    QFile data(":/sheet/style.qss");
    QString style;
    if(data.open(QFile::ReadOnly)){
        std::cout << "hola";
        QTextStream styleIn(&data);
        style = styleIn.readAll();
        data.close();
        qApp->setStyleSheet(style);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BOW w;
    loadStyleSheet();
    w.show();

    return a.exec();
}
