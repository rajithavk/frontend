#include "additems.h"
#include "ui_additems.h"
#include "mainwindow.h"
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

#define Path_to_DB "/home/nalin/Program Works/sqlite3/test.db"

QString fileName;
int row_id;
addItems::addItems(QWidget *parent, int rowid, QString image_path) : QDialog(parent), ui(new Ui::addItems){
    ui->setupUi(this);
    fileName = image_path;
    row_id = rowid;
    if(rowid!=0){
        ui->save->setText("Update");
    }
}

addItems::~addItems(){
    delete ui;
}

void addItems::on_browse_image_clicked(){
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    QPixmap pic(fileName);
    ui->label_image->setPixmap(pic);
//    ui->label_image->setText(fileName);

//    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),"/path/to/file/",tr("Mp3 Files (*.mp3)"));
//    ui->listWidget->addItems(fileNames);

}

void addItems::on_save_clicked(){
    QString name, code, order, price, other;
    name = ui->lineEdit_name->text();
    code = ui->lineEdit_code->text();
    order = ui->lineEdit_order->text();
    price = ui->lineEdit_price->text();
    other = ui->lineEdit_other->text();

    QSqlDatabase dbc = QSqlDatabase::addDatabase("QSQLITE");
    dbc.setDatabaseName(Path_to_DB);
    dbc.open();

    QSqlQuery qry;

    if(row_id==0){
        qry.exec(QString("insert into store values('"+ name +"','"+ fileName +"','"+ order +
                         "','"+ price +"', '"+ code +"', '"+ other +"')"));
        QMessageBox::warning(this,"Message", "New entry was added!");
    }else{
        qry.exec(QString("UPDATE store SET rowid="+QString::number(row_id)+", name='"+ name +"', image='"+ fileName +"', order_no='"+ order +
                         "', price='"+ price +"', code='"+ code +"', other='"+ other +"' WHERE rowid="+QString::number(row_id)+""));
        QMessageBox::warning(this,"Message", "Entry at row id "+ QString::number(row_id) +" was updated!");
    }

}


void addItems::on_Exit_clicked(){
    MainWindow *mw = new MainWindow();
    mw->show();
    close();
}

void addItems::set_lineEdit_name( const QString & text ){
      ui->lineEdit_name->setText( text );
}

void addItems::set_label_image( const QString & text ){
      QPixmap pic(text);
      ui->label_image->setPixmap(pic);
}

void addItems::set_lineEdit_order( const QString & text ){
      ui->lineEdit_order->setText( text );
}

void addItems::set_lineEdit_price( const QString & text ){
      ui->lineEdit_price->setText( text );
}

void addItems::set_lineEdit_code( const QString & text ){
      ui->lineEdit_code->setText( text );
}

void addItems::set_lineEdit_other( const QString & text ){
      ui->lineEdit_other->setText( text );
}
