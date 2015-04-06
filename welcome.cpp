#include "welcome.h"
#include "ui_welcome.h"
#include <QDateTime>

welcome::welcome(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::welcome)
{
    ui->setupUi(this);
    QDateTime dt;
    qDebug() << dt.currentDateTime().toString(Qt::ISODate);
}

welcome::~welcome()
{
    delete ui;
}


void welcome::on_btnExit_clicked()
{
    exit(0);
}

void welcome::on_btnOps_clicked()
{
    bow = new BOW();
    bow->show();
}

void welcome::on_btnAdd_clicked()
{
    invt = new Inventory(0,0);
    invt->show();
}

void welcome::on_btnRun_clicked()
{
    imgcap = new ImageCapture();
    imgcap->show();
}

void welcome::on_btnGrid_clicked()
{

    QPixmap pixmap1("3.jpg");
    QPixmap pixmap2("3.jpg");
    QPixmap pixmap3("3.jpg");
    map<int, QPixmap> pics_temp;
    pics_temp[1] = pixmap1;
    pics_temp[2] = pixmap2;
    pics_temp[3] = pixmap3;
    pics_temp[4] = pixmap3;
    pics_temp[5] = pixmap3;
    pics_temp[6] = pixmap3;
    pics_temp[7] = pixmap3;
    pics_temp[8] = pixmap3;
    pics_temp[9] = pixmap3;
    pics_temp[3] = pixmap3;
    pics_temp[10] = pixmap3;
    pics_temp[11] = pixmap3;
    pics_temp[12] = pixmap3;
    pics_temp[13] = pixmap3;
    pics_temp[14] = pixmap3;
    map<int, QPixmap> *pointer = &pics_temp;
//    QWidget *parent = 0;
    d = new display( 0,pointer);
    d->show();

}
