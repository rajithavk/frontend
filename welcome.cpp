#include "welcome.h"
#include "ui_welcome.h"

welcome::welcome(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::welcome)
{
    ui->setupUi(this);
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
