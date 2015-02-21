#include "imagecapture.h"
#include "ui_imagecapture.h"

ImageCapture::ImageCapture(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageCapture),
    currentGrabber(NULL)
{
    ui->setupUi(this);
    mjpgGrabber = new QImageGrabberMjpeg(this);
    imageGrabbers.append(mjpgGrabber);
    settings.beginGroup("mjpgGrabber");
    mjpgGrabber->setSource(settings.value("url","http://").toString());
    settings.endGroup();

    foreach(QImageGrabber *currentGrabber,imageGrabbers){
        ui->comboBoxGrabberTypes->addItem(currentGrabber->grabberName());
    }

    setImageGrabber(mjpgGrabber);
}

ImageCapture::~ImageCapture()
{
    settings.beginGroup("mjpgGrabber");
    settings.setValue("url",mjpgGrabber->currentSource());
    settings.endGroup();

    delete ui;
}

void ImageCapture::closeEvent(QCloseEvent *event){
    Q_UNUSED(event)
}


void ImageCapture::on_comboBoxGrabberTypes_activated(int index)
{
    setImageGrabber(imageGrabbers.at(index));
}


void ImageCapture::setImageGrabber(QImageGrabber *gb){
    disconnect(this,SLOT(newImageReceived(QImage*)));
    disconnect(this,SLOT(newFPSCalculated(double,double)));

    if(currentGrabber!=NULL){
        currentGrabber->stopGrabbing();
    }

    currentGrabber = gb;
    settingsDialog.setImageGrabber(currentGrabber);
    connect(currentGrabber,SIGNAL(newImageGrabbed(QImage*)),this,SLOT(newImageReceived(QImage*)));
    connect(currentGrabber,SIGNAL(newFPSCalculated(double,double)),this,SLOT(newFPSCalculated(double,double)));
    connect(currentGrabber,SIGNAL(stateChanged(QImageGrabber::GrabbingState)),this,SLOT(grabbingStateChanged(QImageGrabber::GrabbingState)));

    int i = 0;
    foreach(QImageGrabber *cgb, imageGrabbers){
        if(cgb==currentGrabber){
            break;
        }
        i++;
    }
    if(ui->comboBoxGrabberTypes->currentIndex() !=i)
        ui->comboBoxGrabberTypes->setCurrentIndex(i);

}

void ImageCapture::newImageReceived(QImage *img){
    ui->graphicsViewImage->newImageReceieved(img);
}

void ImageCapture::newFPSCalculated(double fps, double avg){
    ui->labelFPSValue->setText(QString::number(fps).leftJustified(7,'0'));
    ui->labelFPSAvgValue->setText(QString::number(avg).leftJustified(7,'0'));
}

void ImageCapture::grabbingStateChanged(QImageGrabber::GrabbingState newState){
    if(newState == QImageGrabber::GrabbingOn)
        ui->pushButtonStart->setText(tr("Stop"));
    else
        ui->pushButtonStart->setText(tr("Start"));
}

void ImageCapture::on_pushButtonStart_clicked()
{
    if(currentGrabber!=NULL){
        if(currentGrabber->isGrabbing()){
            currentGrabber->stopGrabbing();
        }else{
            currentGrabber->startGrabbing();
            ui->graphicsViewImage->grabbingStarted();
        }
    }
}

void ImageCapture::on_pushButtonSettings_clicked(){
    settingsDialog.show();
}


void ImageCapture::on_pushButton_clicked()
{
    if(currentGrabber!=NULL){
        if(currentGrabber->isGrabbing()){

        }
    }
}
