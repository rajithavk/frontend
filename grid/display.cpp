#include "display.h"
#include "ui_display.h"
#include "item_details.h"
#include "qrightclickpushbutton.h"

#include <map>
#include <QSqlDatabase>
#include <QFileInfo>
#include <QSqlQuery>
#include <QMessageBox>
#include <QPushButton>
#include <iostream>
#include <QPixmap>

#define Path_to_DB "dbinventory.db"

using namespace std;

display::display(QWidget *parent, map<int, QPixmap> *pics): QMainWindow(parent), ui(new Ui::display){
    ui->setupUi(this);

    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setCentralWidget(ui->scrollArea);

    int pic_cnt=0;
    this->pics = pics;
    if(this->pics==0){//default state, at the start
        //cout<<"working"<<endl;
        map<int, QPixmap> pics_temp;
        QSqlDatabase dbc = QSqlDatabase::addDatabase("QSQLITE");
        dbc.setDatabaseName(Path_to_DB);
        QFileInfo checkFile (Path_to_DB);


        if(checkFile.isFile()){
            if(dbc.open()){
                QSqlQuery qry;
                qry.exec("SELECT rowid, name, image, order_no, price, code, other FROM store");

                while(qry.next()){
                    QString path_to_pic = qry.value(2).toString();
                    QPixmap pixmap(path_to_pic);
                    pics_temp[++pic_cnt] = pixmap;
                }
                qry.finish();
            }else{
                QMessageBox msgBox;
                msgBox.setText("database file didn't open.........!!!");
                msgBox.exec();
            }
        }else{
            QMessageBox msgBox;
            msgBox.setText("database file isn't exist........!!!");
            msgBox.exec();

        }
        this->pics = &pics_temp;

    }// if for default pics

    dbc.close();
    dbc.removeDatabase("QSQLITE");

    signalMapper_click = new QSignalMapper(this);
    signalMapper_rightclick = new QSignalMapper(this);
    signalMapper_Edit = new QSignalMapper(this);
    signalMapper_Delete = new QSignalMapper(this);
    signalMapper_Replace = new QSignalMapper(this);
    int disp_pic_cnt = 0;
    pic_cnt = pics->size(); // what ever method used to load the pic_cnt this should be needed to get pic count
    for(int row=0; row<=pic_cnt/5; row++){
        int col_limit = pic_cnt-row*5>=5 ? 5 : pic_cnt-row*5;
        for(int col=0; col< col_limit ; col++){
            disp_pic_cnt++;
            QRightClickPushButton *a = new QRightClickPushButton(this);
            a->setFixedSize(200,200);
            QPixmap pic = pics->at(row*5+col+1);
            QIcon ButtonIcon(pic);
            a->setIcon(ButtonIcon);
            a->setIconSize(a->rect().size());

            QMenu *menu = new QMenu(this);
//            add edit menu action
            QAction *Edit = new QAction(tr("&Edit"), this);
            Edit->setStatusTip(tr("Edit store item"));
            menu->addAction(Edit);

//            add delete action
            QAction *Delete = new QAction(tr("&Delete"), this);
//            Delete->setShortcuts(QKeySequence::Delete);
            Delete->setStatusTip(tr("Delete store item"));
            menu->addAction(Delete);

//            add replace menu action
            QAction *Replace = new QAction(tr("&Replace"), this);
            Replace->setStatusTip(tr("Replace store item with another"));
            menu->addAction(Replace);

//            set menu to the buttons
            a->setMenu(menu);

            ui->gridLayout->addWidget(a,row+1,col+1);
//            add action for a button events - left and right click
            connect(a, SIGNAL(clicked()), signalMapper_click, SLOT(map())); // connect signal with signal mapper
            signalMapper_click->setMapping(a, QString::number(row*5+col+1));// mapping parameters
            connect(a, SIGNAL(rightClicked()), signalMapper_rightclick, SLOT(map()));
            signalMapper_rightclick->setMapping(a, QString::number(row*5+col+1));

//            add action for menu events
//            Mapping action Edit menu item
            connect(Edit, SIGNAL(triggered()), signalMapper_Edit, SLOT(map()));
            signalMapper_Edit->setMapping(Edit, QString::number(row*5+col+1));

//            Mapping action Delete menu item
            connect(Delete, SIGNAL(triggered()), signalMapper_Delete, SLOT(map()));
            signalMapper_Delete->setMapping(Delete, QString::number(row*5+col+1));

//            Mapping action Delete menu item
            connect(Replace, SIGNAL(triggered()), signalMapper_Replace, SLOT(map()));
            signalMapper_Replace->setMapping(Replace, QString::number(row*5+col+1));
        }
    }
    ui->gridLayout_2->setSpacing(50);
    connect(signalMapper_click, SIGNAL(mapped(const QString &)), this, SLOT(butgrid(const QString &)));// map with final function
    connect(signalMapper_rightclick, SIGNAL(mapped(const QString &)), this, SLOT(butgrid(const QString &)));
    connect(signalMapper_Edit, SIGNAL(mapped(const QString &)), this, SLOT(butgrid(const QString &)));
    connect(signalMapper_Delete, SIGNAL(mapped(const QString &)), this, SLOT(Delete(const QString &)));
    connect(signalMapper_Replace, SIGNAL(mapped(const QString &)), this, SLOT(Replace(const QString &)));
    connect(signalMapper_Edit, SIGNAL(mapped(const QString &)), this, SLOT(Edit(const QString &)));
}

display::~display(){

    delete ui;
}

void display::butgrid(const QString &passed){
    dbc.setDatabaseName(Path_to_DB);
        map<int, QPixmap> pics_temp;
        QSqlDatabase dbc = QSqlDatabase::addDatabase("QSQLITE");
        dbc.setDatabaseName(Path_to_DB);
        QFileInfo checkFile (Path_to_DB);


        if(checkFile.isFile()){
            if(dbc.open()){
                QSqlQuery qry;
                qry.exec("SELECT rowid, name, image, order_no, price, code, other FROM store WHERE rowid="+passed);
                while(qry.next()){
                    item_details *details = new item_details();
                    details->set_label_name(qry.value(1).toString());
                    details->set_label_image(qry.value(2).toString());
                    details->set_label_order(qry.value(3).toString());
                    details->set_label_price(qry.value(4).toString());
                    details->set_label_code(qry.value(5).toString());
                    details->set_label_other(qry.value(6).toString());
                    details->show();
                }

            }else{
                QMessageBox msgBox;
                msgBox.setText("database file didn't open.........!!!");
                msgBox.exec();
            }
        }else{
            QMessageBox msgBox;
            msgBox.setText("database file isn't exist........!!!");
            msgBox.exec();

        }
        this->pics = &pics_temp;
    this->pics->find(1);
    dbc.close();
    dbc.removeDatabase("QSQLITE");
}

void display::Delete(const QString &passed){
    QMessageBox msgBox;
    msgBox.setText(passed);
    msgBox.exec();
}

void display::Replace(const QString &passed){
    QMessageBox msgBox;
    msgBox.setText(passed);
    msgBox.exec();
}

void display::Edit(const QString &passed)
{
    Inventory *inv = new Inventory(passed.toInt(),0);
    inv->show();
}
