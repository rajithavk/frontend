#include "items_details.h"
#include "ui_items_details.h"

items_details::items_details(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::items_details)
{
    ui->setupUi(this);
}

items_details::~items_details()
{
    delete ui;
}
