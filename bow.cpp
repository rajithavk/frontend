#include "bow.h"
#include "ui_bow.h"
#include "functions.hpp"
#include <QMessageBox>
#include <QComboBox>
#include <QFileDialog>
#include "CONV.h"

vision *bow;

BOW::BOW(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BOW)
{
    ui->setupUi(this);
    bow = new vision();

    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->statusBar->showMessage("Ready");

}

BOW::~BOW()
{
    delete ui;
}

void BOW::on_pushButton_clicked()
{   ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->comboBox->clear();
    //ui->listView->setViewMode(QListView::IconMode);
    ui->listView->setFlow(QListView::TopToBottom);

    bow->clearTrainingSet();
    if((bow->loadTrainingSet())==0){
        QMessageBox msg;
        msg.setText("Loading Successful!");
        msg.setIcon(QMessageBox::Information);
        msg.exec();
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        for(vector<string>::iterator it=bow->classes.begin();it!=bow->classes.end();++it){
            ui->comboBox->addItem(QString::fromStdString(*it));

        on_comboBox_activated(ui->comboBox->currentText());
        }

    }
}

void BOW::on_pushButton_2_clicked()
{
    QMessageBox msg;
    msg.setText("Do you Want to continue with creating vocabulary?");
    msg.setInformativeText("This may take a while to complete. Are you sure?");
    msg.setIcon(QMessageBox::Question);
    msg.setStandardButtons(QMessageBox::Abort|QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Abort);
    int ret = msg.exec();
    switch (ret) {
    case QMessageBox::Abort : break;
    case QMessageBox::Ok    : {
        ui->statusBar->showMessage("Busy..");
        ui->pushButton_2->setEnabled(false);
        bow->buildVocabulary();
        ui->statusBar->showMessage("Ready");
        break;
    }
    default:
        break;
    }
}

void BOW::on_pushButton_3_clicked()
{
    ui->statusBar->showMessage("Busy..");
    if(bow->trainSVM()==0){
        QMessageBox msg;
        msg.setText("SVM Training Successful!");
        msg.setIcon(QMessageBox::Information);
        msg.exec();
    }else{
        QMessageBox msg;
        msg.setText("SVM Training unsuccessful!");
        msg.setInformativeText("Are you Sure you have a vocabulary?");
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
    }
    ui->statusBar->showMessage("Ready");
}

void BOW::on_pushButton_4_clicked()
{
    if(bow->initClassifiers()==0){
        QMessageBox msg;
        msg.setText("SVM Loading Successful!");
        msg.setIcon(QMessageBox::Information);
        msg.exec();
    }else{
        QMessageBox msg;
        msg.setText("SVM Loading unsuccessful!");
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
    }
}

void BOW::on_pushButton_5_clicked()
{
    if(bow->initVocabulary()==0){
        QMessageBox msg;
        msg.setText("Vocabulary Loading Successful!");
        msg.setIcon(QMessageBox::Information);
        msg.exec();
    }else{
        QMessageBox msg;
        msg.setText("Vocabulary Loading unsuccessful!");
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
    }
}


void BOW::on_comboBox_activated(const QString &arg1)
{       multimap<string,Mat> test = bow->getTrainingSet();
        pair    <multimap<string,Mat>::iterator,multimap<string,Mat>::iterator> ret;
        ret = test.equal_range(arg1.toStdString());
        QVector<QPair<QString,QPixmap>> ims;
        for(multimap<string,Mat>::iterator it=ret.first;it!=ret.second;++it){
            ims.push_back(QPair<QString,QPixmap>(QString::fromStdString(it->first),cvMatToQPixmap(it->second)));
        }

        ui->listView->setModel(new imagelistmodel(ims,ui->listView));
        ui->listView->setUpdatesEnabled(true);

}

void BOW::on_pushButton_6_clicked()
{
    try {
        QUrl filename = QFileDialog::getOpenFileUrl(this,"Select an Image",QDir::currentPath(),tr("Images (*.*)"));
        Mat im = imread(filename.path().toStdString(),0);
        bow->testImage(im);
        cv::resize(im,im,Size(im.cols/2,im.rows/2));
        imshow("im",im);
    } catch (Exception e) {
        qDebug() << QString::fromStdString(e.err);
    }
}

void BOW::on_btnTestFolder_clicked()
{
    qDebug() << "Triggered";
   // try {
        QUrl url = QFileDialog::getExistingDirectoryUrl(this,"Select Directory",QDir::currentPath(),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
        QDir dir(url.path());
        dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
        QStringList dirList = dir.entryList();
        //qDebug() << dirList.count();
        foreach (QString i ,dirList) {
            qDebug() << i;
        }

        if(!dirList.count()>0)  return;


  //  } catch (Exception e) {
  //  }


}
