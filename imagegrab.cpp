#include "imagegrab.h"
#include "ui_imagegrab.h"

Imagegrab::Imagegrab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imagegrab)
{
    ui->setupUi(this);
}

Imagegrab::~Imagegrab()
{
    delete ui;
}
