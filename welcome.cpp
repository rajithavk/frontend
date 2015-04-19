#include "welcome.h"
#include "ui_welcome.h"
#include <QDateTime>

welcome::welcome(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::welcome)
{
    ui->setupUi(this);

    QDateTime dt;
    qDebug() << dt.currentDateTime().toString(Qt::ISODate);

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
    imgcap = new ImageCapture(0);
    imgcap->show();
    connect(imgcap,SIGNAL(call_grid(map<int,QPixmap>)),this,SLOT(setFocus()));
}

void welcome::on_btnGrid_clicked()
{
    getRecordsForGrid("dbinventory.db");
    multimap<int,QPixmap> *pointer = mp;
    d = new display( 0,pointer);
    d->show();

}


void welcome::getRecordsForGrid(const QString &fileName){

    QSqlDatabase dbc = QSqlDatabase::addDatabase("QSQLITE");

    dbc.setDatabaseName(fileName);

        if(dbc.open()){
            QSqlQuery *qry = new QSqlQuery(dbc);
            qry->prepare("SELECT id, image FROM items");
            qry->exec();
            int id = 0;
            mp = new multimap<int,QPixmap>;
            while(qry->next()){
                id = qry->value(0).toInt();
                QPixmap pic = QPixmap(qry->value(1).toString());
                //qDebug() << id;
                mp->insert(pair<int,QPixmap>(id,pic));
                //std::cout << qry->value(1).toString().toStdString() << std::endl;
            }
        }else{
            qDebug() << "Can't Open the DB";
        }
}

void welcome::on_btnStream_clicked()
{
    QSettings settings;
    settings.beginGroup("mjpgGrabber");
    QString URL = settings.value("url").toString();
    //QString URL = "http://10.8.144.82:8080/stream_viewer?topic=/camera/image";
    settings.endGroup();
    capture = new cv::VideoCapture();


    //if(vcap.open(URL.toLocal8Bit().data())) qDebug() << "TRUE";
    if(capture->open(URL.toStdString())){
        if(capture->isOpened()) qDebug() << "Stream Pre-Opened";
        capture->grab();
        QMessageBox msg;
        msg.setText("Successfully Connected to Camera Stream");
        msg.setIcon(QMessageBox::Information);
        msg.exec();
    }
}

void welcome::on_call_for_inventory(multimap<int, QPixmap> res)
{
//    multimap<int,QPixmap> *m = &res;
//    //qDebug() << "Signal Emited to get the display";
//    qDebug() << m->size();

//    display *dis= new display(0,m);
//    dis->show();
}
