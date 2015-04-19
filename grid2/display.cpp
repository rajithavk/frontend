#include "display.h"
#include "ui_display.h"
#include "additems.h"
#include "item_details.h"
#include "qrightclickpushbutton.h"
#include "selectgrid.h"

#include <QMultiMap>
#include <QSqlDatabase>
#include <QFileInfo>
#include <QSqlQuery>
#include <QMessageBox>
#include <QPushButton>
#include <iostream>
#include <QPixmap>
#include <QList>

#define Path_to_DB "test.db"

using namespace std;
using namespace results_grid;
display::display(QWidget *parent, QMultiMap<int, vector<pair<string, float> > > *pics): QMainWindow(parent), ui(new Ui::display), pics_(pics){
    ui->setupUi(this);

    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setCentralWidget(ui->scrollArea);

    int pic_cnt=0;
    QList <int>keys = pics->keys();
    QMultiMap<int, QPixmap> pics_tem;
    for(int cnt=0; cnt<keys.length(); cnt++){
        QPixmap pic("BOW/recent/"+QString::number(keys.at(cnt))+".jpg");
        pics_tem.insert(keys.at(cnt),pic);
    }
//    keys.at(0);
//    this->pics = &pics_tem;
//    if(this->pics==0){//default state, at the start
//        //cout<<"working"<<endl;
//        map<int, QPixmap> pics_temp;
//        QSqlDatabase dbc = QSqlDatabase::addDatabase("QSQLITE");
//        dbc.setDatabaseName(Path_to_DB);
//        QFileInfo checkFile (Path_to_DB);


//        if(checkFile.isFile()){
//            if(dbc.open()){
//                QSqlQuery qry;
//                qry.exec("SELECT rowid, name, image, order_no, price, code, other FROM store");

//                while(qry.next()){
//                    QString path_to_pic = qry.value(2).toString();
////                    QPixmap pixmap(path_to_pic); "/home/nalin/test1/untitled/2.jpg"
//                    QPixmap pixmap("/home/nalin/test1/untitled/2.jpg");
//                    pics_temp[++pic_cnt] = pixmap;
//                }

//            }else{
//                QMessageBox msgBox;
//                msgBox.setText("database file didn't open.........!!!");
//                msgBox.exec();
//            }
//        }else{
//            QMessageBox msgBox;
//            msgBox.setText("database file isn't exist........!!!");
//            msgBox.exec();

//        }
//        this->pics = &pics_temp;
//    }// if for default pics

    signalMapper_click = new QSignalMapper(this);
    signalMapper_rightclick = new QSignalMapper(this);
    signalMapper_Edit = new QSignalMapper(this);
    signalMapper_Delete = new QSignalMapper(this);
    signalMapper_Replace = new QSignalMapper(this);

    int disp_pic_cnt = 0;
    pic_cnt = pics_tem.size(); // what ever method used to load the pic_cnt this should be needed to get pic count
    QMap<int,QPixmap>::iterator it = pics_tem.begin();
    for(int row=0; row<=pic_cnt/5; row++){
        int col_limit = pic_cnt-row*5>=5 ? 5 : pic_cnt-row*5;
        for(int col=0; col< col_limit ; col++){
            disp_pic_cnt++;
            QRightClickPushButton *a = new QRightClickPushButton(this);
            a->setFixedSize(200,200);//cout<<this->pics->size()<<endl;
//            QPixmap pic("/home/nalin/test1/untitled/2.jpg");
//            QPixmap pixmap(juwelaries.at(row*5+col).value(2).toString());
//            cout<<this->pics->size()<<endl;
//            map<int,QPixmap>::iterator it = pics->find(keys(row*5+col+1));
            QPixmap pic = *it;
            it++;
//            QPixmap pic = this->pics->at(keys(row*5+col+1));
            QIcon ButtonIcon(pic);
            a->setIcon(ButtonIcon);
            a->setIconSize(pic.rect().size());

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
//            butgrid.append(a);// add buttons to qvector
        }

    }

    ui->gridLayout_2->setSpacing(50);
//    connect(signalMapper_click, SIGNAL(mapped(const QString &)), this, SLOT(butgrid(const QString &)));// map with final function
    connect(signalMapper_rightclick, SIGNAL(mapped(const QString &)), this, SLOT(select_grid(const QString &)));
    connect(signalMapper_Edit, SIGNAL(mapped(const QString &)), this, SLOT(butgrid(const QString &)));
    connect(signalMapper_Delete, SIGNAL(mapped(const QString &)), this, SLOT(Delete(const QString &)));
    connect(signalMapper_Replace, SIGNAL(mapped(const QString &)), this, SLOT(select_grid(const QString &)));

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
//        this->pics = &pics_temp;

//    this->pics->find(1);
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

void display::select_grid(const QString &passed){
    vector <pair<string, float> > temp_vector;
    temp_vector = pics_->value(passed.toInt());
    std::vector<pair<string, float> >::iterator it = temp_vector.begin();

    QString first_image_name =  QString::fromStdString((*it).first);
    float first_image_float =  (*it).second;it++;
    QString second_image_name =  QString::fromStdString((*it).first);
    float second_image_float =  (*it).second;it++;
    QString third_image_name =  QString::fromStdString((*it).first);
    float third_image_float =  (*it).second;

    selectgrid *s = new selectgrid(0, passed, first_image_name, second_image_name, third_image_name,first_image_float,second_image_float,third_image_float);
    s->show();
}
