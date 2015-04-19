#include "selectgrid.h"
#include "ui_selectgrid.h"
//QMultiMap<int,vector<pair<string,float>>> results;

selectgrid::selectgrid(QWidget *parent, QString mainIm, QString sIm1, QString sIm2, QString sIm3, float fIm1, float fIm2, float fIm3) : QMainWindow(parent), ui(new Ui::selectgrid), mainIm(mainIm),
sIm1(sIm1),sIm2(sIm2),sIm3(sIm3),fIm1(fIm1),fIm2(fIm2),fIm3(fIm3){
    ui->setupUi(this);

    QPixmap pic4("BOW/Recent/A/"+this->mainIm+".jpg");
    QIcon ButtonIcon4(pic4);
    ui->pushButton_4->setIcon(ButtonIcon4);
    ui->pushButton_4->setIconSize(ui->pushButton_4->rect().size());

    QPixmap pic("BOW/images/"+this->sIm1+"/10.jpg");
    QIcon ButtonIcon(pic);
    ui->pushButton->setIcon(ButtonIcon);
    ui->pushButton->setIconSize(ui->pushButton->rect().size());
    ui->label->setText(QString::number(this->fIm1));

    QPixmap pic2("BOW/images/"+this->sIm2+"/10.jpg");
    QIcon ButtonIcon2(pic2);
    ui->pushButton_2->setIcon(ButtonIcon2);
    ui->pushButton_2->setIconSize(ui->pushButton_2->rect().size());
    ui->label_2->setText(QString::number(this->fIm2));

    QPixmap pic3("BOW/images/"+this->sIm3+"/10.jpg");
    QIcon ButtonIcon3(pic3);
    ui->pushButton_3->setIcon(ButtonIcon3);
    ui->pushButton_3->setIconSize(ui->pushButton_3->rect().size());
    ui->label_3->setText(QString::number(this->fIm3));
}

selectgrid::~selectgrid(){
    delete ui;
}

void selectgrid::on_pushButton_clicked(){

}

void selectgrid::on_pushButton_2_clicked(){

}

void selectgrid::on_pushButton_3_clicked(){

}
