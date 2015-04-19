#include "item_details.h"
#include "ui_item_details.h"

//#include <

item_details::item_details(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::item_details){
    ui->setupUi(this);
}

item_details::~item_details(){
    delete ui;
}


void item_details::set_label_name ( const QString & text ){
    ui->label_name->setText(text);
}
void item_details::set_label_image ( const QString & text ){
    QPixmap pic(text);
    ui->label_image->setPixmap(pic);
}
void item_details::set_label_order ( const QString & text ){
    ui->label_order->setText(text);
}
void item_details::set_label_price ( const QString & text ){
    ui->label_price->setText(text);
}
void item_details::set_label_code ( const QString & text ){
    ui->label_code->setText(text);
}
void item_details::set_label_other ( const QString & text ){
    ui->label_other->setText(text);
}
