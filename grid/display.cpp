#include "display.h"
#include "ui_display.h"
//#include "additems.h"
#include "item_details.h"
#include "qrightclickpushbutton.h"

#include <map>
#include <QSqlDatabase>
#include <QFileInfo>
#include <QSqlQuery>
#include <QMessageBox>
#include <QPushButton>
#include <iostream>

#define Path_to_DB "test.db"

using namespace std;

display::display(QWidget *parent, map<int, QPixmap> *pics):
    QMainWindow(parent),
    ui(new Ui::display){
    ui->setupUi(this);

    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setCentralWidget(ui->scrollArea);

    int pic_cnt=0;
    this->pics = pics;
    if(this->pics==0){//default state, at the start
        cout<<"working"<<endl;
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

    signalMapper_click = new QSignalMapper(this);
    signalMapper_rightclick = new QSignalMapper(this);
//    puts(pics-> size());
    int disp_pic_cnt = 0;
    pic_cnt = pics->size(); // what ever method used to load the pic_cnt this should be needed to get pic count
    for(int row=0; row<=pic_cnt/5; row++){
        int col_limit = pic_cnt-row*5>=5 ? 5 : pic_cnt-row*5;
        for(int col=0; col< col_limit ; col++){
            disp_pic_cnt++;
            QRightClickPushButton *a = new QRightClickPushButton(this);
            a->setFixedSize(200,200);
//            QPixmap pixmap("/home/nalin/test1/untitled/2.jpg");
//            QPixmap pixmap(juwelaries.at(row*5+col).value(2).toString());
            QPixmap pic = pics->at(row*5+col+1);
//            QIcon ButtonIcon(pixmap);
            QIcon ButtonIcon(pic);
            a->setIcon(ButtonIcon);
            a->setIconSize(pic.rect().size());

            QMenu *menu = new QMenu(this);
            menu->addAction("Edit");
            menu->addAction("Delete");
            menu->addAction("Replace");
            a->setMenu(menu);

            ui->gridLayout->addWidget(a,row+1,col+1);
            connect(a, SIGNAL(clicked()), signalMapper_click, SLOT(map())); // connect signal with signal mapper
            signalMapper_click->setMapping(a, QString::number(row*5+col+1));// mapping parameters
            connect(a, SIGNAL(rightClicked()), signalMapper_rightclick, SLOT(map()));
            signalMapper_rightclick->setMapping(a, QString::number(row*5+col+1));

//            butgrid.append(a);// add buttons to qvector
        }

    }

    ui->gridLayout_2->setSpacing(50);
    connect(signalMapper_click, SIGNAL(mapped(const QString &)), this, SLOT(butgrid(const QString &)));// map with final function
    connect(signalMapper_rightclick, SIGNAL(mapped(const QString &)), this, SLOT(butgrid(const QString &)));
}

display::~display(){
    delete ui;
}

void display::butgrid(const QString &passed){
//    dbc = QSqlDatabase::addDatabase("QSQLITE");
    dbc.setDatabaseName(Path_to_DB);
//    QFileInfo checkFile (Path_to_DB);


        map<int, QPixmap> pics_temp;
        QSqlDatabase dbc = QSqlDatabase::addDatabase("QSQLITE");
        dbc.setDatabaseName(Path_to_DB);
        QFileInfo checkFile (Path_to_DB);


        if(checkFile.isFile()){
            if(dbc.open()){
                QSqlQuery qry;
//                QString query = QString ("SELECT rowid, name, image, order_no, price, code, other FROM store WHERE rowid=%1").arg(1);
                qry.exec("SELECT rowid, name, image, order_no, price, code, other FROM store WHERE rowid="+passed);
//                qry.exec(query);
//                puts("test");
                while(qry.next()){
//                    QString path_to_pic = qry.value(1).toString();
//                    puts(path_to_pic.toLocal8Bit().data());// QString to char array convert

                    item_details *details = new item_details();
                    details->set_label_name(qry.value(1).toString());
                    details->set_label_image(qry.value(2).toString());
                    details->set_label_order(qry.value(3).toString());
                    details->set_label_price(qry.value(4).toString());
                    details->set_label_code(qry.value(5).toString());
                    details->set_label_other(qry.value(6).toString());
//                        addItems *showItem = new addItems(0, passed.toInt(), qry.value(2).toString());
//                        showItem->set_lineEdit_name(qry.value(1).toString());
                    //    showItem->set_label_image(juwelaries.at(passed.toInt()-1).value(2).toString());
                    //    showItem->set_lineEdit_order(juwelaries.at(passed.toInt()-1).value(3).toString());
                    //    showItem->set_lineEdit_price(juwelaries.at(passed.toInt()-1).value(4).toString());
                    //    showItem->set_lineEdit_code(juwelaries.at(passed.toInt()-1).value(5).toString());
                    //    showItem->set_lineEdit_other(juwelaries.at(passed.toInt()-1).value(6).toString());

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
//    int rowid = juwelaries.at(passed.toInt()-1).value(0).toInt();
//    QString image_path = juwelaries.at(passed.toInt()-1).value(2).toString();

//    addItems *showItem = new addItems(0, rowid, image_path);
//    showItem->set_lineEdit_name(juwelaries.at(passed.toInt()-1).value(1).toString());
//    showItem->set_label_image(juwelaries.at(passed.toInt()-1).value(2).toString());
//    showItem->set_lineEdit_order(juwelaries.at(passed.toInt()-1).value(3).toString());
//    showItem->set_lineEdit_price(juwelaries.at(passed.toInt()-1).value(4).toString());
//    showItem->set_lineEdit_code(juwelaries.at(passed.toInt()-1).value(5).toString());
//    showItem->set_lineEdit_other(juwelaries.at(passed.toInt()-1).value(6).toString());

//    showItem->show();
//    this->close();
}
