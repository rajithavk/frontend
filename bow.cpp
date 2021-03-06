#include "bow.h"
#include "ui_bow.h"
#include "functions.hpp"
#include <QMessageBox>
#include <QComboBox>
#include <QFileDialog>
#include "CONV.h"
#include "QStringList"
#include "ResultsGrid/display2.h"

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
   initSVMs(0);
}

void BOW::on_pushButton_5_clicked()
{
    initVocabulary(0);
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
   try {
        QUrl url = QFileDialog::getExistingDirectoryUrl(this,"Select Directory",QDir::currentPath(),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
        QMultiMap<int,vector<pair<string,float>>> results = testFolder(url,1);

        saveXMLFile("betaresults",results);
        display2 *nd = new display2(0,&results);
        nd->show();

  } catch (Exception e) {
        qDebug() << QString::fromStdString(e.err);
  }


}

QMultiMap<int,vector<pair<string,float>>> BOW::testFolder(QUrl url,bool mode){
    QMultiMap<int,vector<pair<string,float>>> results;
    QDir dir(url.path());
    dir.setSorting(QDir::LocaleAware);
    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    QStringList dirList = dir.entryList();
    //qDebug() << dirList.count();

    if(!dirList.count()>0)  return results;
    QDir ims;
    QStringList imTmp;
    ims.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    foreach (QString i ,dirList) {
        qDebug() << i;
        ims.setPath(dir.path()+"/"+i);
        ims.setNameFilters(QStringList() << "*.jpg" << "*.JPG");
        imTmp = ims.entryList();
        QStringList imList;
        foreach (QString x, imTmp) {
            imList.push_back(x.left(x.length()-4));
        }
        qSort(imList.begin(),imList.end(),compareid);
        foreach (QString a, imList) {
            // qDebug() << ims.path()+"/"+a;
            QString impath = ims.path() +"/" + a + ".jpg";
            qDebug() << impath;
            try {
                if(mode)
                    results.insert(a.toInt(),bow->testImage(impath.toStdString()));
                 else
                    results.insert(i.toInt(),bow->testImage(impath.toStdString()));

                qDebug() << "Result Added";

            } catch (exception e) {
                qDebug() << e.what();
                qDebug() << "Error Here";
            }
        }
    }

    return results;
}




void BOW::saveXMLFile(QString fileName, QMultiMap<int, vector<pair<string, float> > > &results)
{
 QFile file(QDir::currentPath() + "/BOW/" + fileName +".xml");
 if(!file.open(QIODevice::WriteOnly)) return;

 QXmlStreamWriter xmlWriter(&file);

 qDebug() << "File opened";

 xmlWriter.setAutoFormatting(true);
 xmlWriter.writeStartDocument();
 xmlWriter.writeStartElement("Results");
 try{
 for(QMultiMap<int,vector<pair<string,float > > >::iterator it = results.begin();it!=results.end();it++){
     vector<pair<string,float>>::iterator itv = it.value().begin();
   // if((it) == NULL) continue;

         xmlWriter.writeStartElement("Item");
         xmlWriter.writeAttribute("Original",QString::number(it.key()));
             xmlWriter.writeStartElement("R1");
                xmlWriter.writeAttribute("id",QString::fromStdString((*itv).first));
                xmlWriter.writeAttribute("score",QString::number(((*itv).second)));
             xmlWriter.writeEndElement();
             xmlWriter.writeStartElement("R2");
                 xmlWriter.writeAttribute("id",QString::fromStdString((*(itv+1)).first));
                 xmlWriter.writeAttribute("score",QString::number(((*(itv+1)).second)));
             xmlWriter.writeEndElement();
             xmlWriter.writeStartElement("R3");
                 xmlWriter.writeAttribute("id",QString::fromStdString((*(itv+2)).first));
                 xmlWriter.writeAttribute("score",QString::number(((*(itv+2)).second)));
             xmlWriter.writeEndElement();

         xmlWriter.writeEndElement();
 }
 xmlWriter.writeEndElement();
    qDebug() << "XML Succefully Written";

 } catch (Exception e) {
     qDebug() << QString::fromStdString(e.err);
 }
 file.close();
}

void BOW::initVocabulary(bool s)
{
    if(bow->initVocabulary()==0){
        if(s){
            qDebug() << "Vocabulary Loading Successful!";
        }else{
            QMessageBox msg;
            msg.setText("Vocabulary Loading Successful!");
            msg.setIcon(QMessageBox::Information);
            msg.exec();
        }
    }else{
        if(s){
            qDebug() << "Vocabulary Loading unsuccessful!";
        }else{
            QMessageBox msg;
            msg.setText("Vocabulary Loading unsuccessful!");
            msg.setIcon(QMessageBox::Critical);
            msg.exec();
        }
    }
}

void BOW::initSVMs(bool s)
{
    if(bow->initClassifiers()==0){
        if(s){
            qDebug() << "SVM Loading Successful!";
        }else{
            QMessageBox msg;
            msg.setText("SVM Loading Successful!");
            msg.setIcon(QMessageBox::Information);
            msg.exec();
        }
    }else{
        if(s){
            qDebug() << "SVM Loading unsuccessful!";
        }else{
            QMessageBox msg;
            msg.setText("SVM Loading unsuccessful!");
            msg.setIcon(QMessageBox::Critical);
            msg.exec();
        }
    }
}
