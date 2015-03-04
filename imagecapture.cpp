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
    mjpgGrabber->setSource(settings.value("url").toString());
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
    if(imageSet.size()>0){
        emit gotImageSet(imageSet);
        qDebug() << "imageset emmited";
    }

    settings.beginGroup("mjpgGrabber");
    settings.setValue("url",mjpgGrabber->currentSource());
    settings.endGroup();
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
    QImage timg;
    if(ui->chkFilp->isChecked())
        timg =  img->mirrored(false,true);
    else
        timg = img->copy();

    ui->graphicsViewImage->newImageReceieved(&timg);
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
            QPixmap img = QPixmap::fromImage(ui->graphicsViewImage->imageItem->getImage());
            imageSet.append(img);
            ui->labelLastImage->setPixmap(img);
        }
    }
}

void ImageCapture::on_btnConnect_clicked()
{
    robot.open("ws://10.0.0.1:9090");
}

void ImageCapture::on_btnRight_clicked()
{
    robot.publish("right");
}

void ImageCapture::on_btnForward_clicked()
{
    robot.publish("go");
}

void ImageCapture::on_btnLeft_clicked()
{
    robot.publish("left");
}

void ImageCapture::on_btnBack_clicked()
{
    robot.publish("back");
}

void ImageCapture::on_btnStop_clicked()
{
    robot.publish("stop");
}

void ImageCapture::gotConState(QString state)
{
    if(state=="Connected")
        ui->btnConnect->setText("Disconnect");
    else
        ui->btnConnect->setText("Connect");
}

void ImageCapture::on_pushButton_2_clicked()
{
    robot.publish("run");
    connect(&robot, SIGNAL(snapIt()),this,SLOT(snapIt()));
    connect(&robot,SIGNAL(robotSaid(QString)),this,SLOT(fromRobot(QString)));
}

void ImageCapture::snapIt()
{
    //QPixmap snap(QPixmap::fromImage(ui->graphicsViewImage->imageItem->getImage()));
    vector<Mat> temp;
    temp = convert(ui->graphicsViewImage->imageItem->getImage());
    testImages.insert(testImages.end(),temp.begin(),temp.end());

}

void ImageCapture::fromRobot(QString m)
{
    QMessageBox msg;
    msg.setWindowTitle("Finished");
    msg.setText("Autorun Finished");
    msg.setIcon(QMessageBox::Information);
    msg.exec();
}
