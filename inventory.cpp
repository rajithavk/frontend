#include "inventory.h"
#include "ui_inventory.h"

inventory::inventory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inventory)
{
    ui->setupUi(this);
}

inventory::~inventory()
{
    delete ui;
}
