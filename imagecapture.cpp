#include "imagecapture.h"
#include "ui_imagecapture.h"
#include "grid/display.h"
//ImageCapture::ImageCapture(QWidget *parent,cv::VideoCapture *c) :
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
    URL = settings.value("url").toString();
    settings.endGroup();


    foreach(QImageGrabber *currentGrabber,imageGrabbers){
        ui->comboBoxGrabberTypes->addItem(currentGrabber->grabberName());
    }
    ui->comboBoxGrabberTypes->addItem("OpenCV Grabber");

    setImageGrabber(mjpgGrabber);
    connect(&robot,SIGNAL(conState(QString)),this,SLOT(connectionState(QString)));
    imgCount = 1;
    cvGrabber = false;


    ////////////////////////////////////////////////////

//    BOW *b = new BOW();
//    b->initVocabulary(1);
//    b->initSVMs(1);

//    QMultiMap<int,vector<pair<string,float>>> tmpres = b->testFolder(QUrl(QDir::currentPath()+"/BOW/Recent"),1);
//    b->saveXMLFile("Recent/A/" + QDateTime::currentDateTime().toString(Qt::ISODate),tmpres);

//    multimap<int,QPixmap> res;
//    for(QMultiMap<int,vector<pair<string,float > > >::iterator it=tmpres.begin();it!=tmpres.end();it++){
//        vector<pair<string,float>>::iterator itv = it.value().begin();
//        //qDebug() << QDir::currentPath()+"/BOW/images/"+ QString::fromStdString((*itv).first) + "/10.jpg";
//        QPixmap mp(QDir::currentPath()+"/BOW/images/"+ QString::fromStdString((*itv).first) + "/10.jpg");
//        qDebug() << QString::fromStdString((*itv).first).toInt();
//        res.insert(pair<int,QPixmap>(QString::fromStdString((*itv).first).toInt(),mp));
//    }
//    //emit call_grid(res);
//    multimap<int,QPixmap> *m = &res;
//    //qDebug() << "Signal Emited to get the display";
//    qDebug() << m->size();

//    display *dis= new display(0,m);
//    dis->show();

    ///////////////////////////////////////////////////

}

ImageCapture::~ImageCapture()
{
    cvIsGrabbing = false;
    settings.beginGroup("mjpgGrabber");
    settings.setValue("url",mjpgGrabber->currentSource());
    settings.endGroup();

    delete ui;
}

void ImageCapture::closeEvent(QCloseEvent *event){
    Q_UNUSED(event)

    cvIsGrabbing = false;
    vcap.release();
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
    if(index==0){
     setImageGrabber(imageGrabbers.at(index));
     cvGrabber = false;
    }
    else{
            qDebug() << "OpenCV Grabber Selected";
            if(capture!=NULL){
                vcap = *capture;
            }else{
                qDebug() << "Connect to stream first";
                return;
            }
            connect(this,SIGNAL(newOpencvGrabbedImage(QImage*)),this,SLOT(newImageReceived(QImage*)));
            //if(vcap.open(URL.toStdString())){
            if(vcap.isOpened()){
                cvIsGrabbing = false;
                cvGrabber = true;
                qDebug() << "Connected to Stream";
            }
            else    qDebug() << "Cannot Open the URL for OpenCV grabber";
    }
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

void ImageCapture::opencvGrabImage()
{
    qDebug() << "Here we go";
    while(cvIsGrabbing){
        if(vcap.read(image)){
            Rect rect(200,0,400,500);
            image = image(rect);
            //qDebug() << segment(image);
            cvImage = new QImage(cvMatToQImage(image));
            emit newOpencvGrabbedImage(cvImage);

        }
    }
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

void ImageCapture::connectionState(QString m)
{
    if(m == "Connected")
        ui->btnConnect->setText("Disconnect");
    else
        ui->btnConnect->setText("Connect");
}

void ImageCapture::on_pushButtonStart_clicked()
{
    if(cvGrabber){
        qDebug() << "Going Concurrent";
        if(cvIsGrabbing){
            cvIsGrabbing = false;
            ui->pushButtonStart->setText("Start");
        }
        else{
            cvIsGrabbing = true;
            ui->pushButtonStart->setText("Stop");
        }
        QFuture<void> future = QtConcurrent::run(this,&ImageCapture::opencvGrabImage);
    }
    else{
        if(currentGrabber!=NULL){
        if(currentGrabber->isGrabbing()){
            currentGrabber->stopGrabbing();
        }else{
            currentGrabber->startGrabbing();
            ui->graphicsViewImage->grabbingStarted();
        }
    }
    }
}

void ImageCapture::on_pushButtonSettings_clicked(){
    settingsDialog.show();
}


void ImageCapture::on_pushButton_clicked()
{
    if(cvGrabber){
        QPixmap img = QPixmap::fromImage(ui->graphicsViewImage->imageItem->getImage());
        imageSet.append(img);
        ui->labelLastImage->setPixmap(img);
        ui->labelLastImage->setScaledContents(true);
        ui->pushButton->setText("Capture " + QString::number(imgCount++));
    }else{
    if(currentGrabber!=NULL){
        if(currentGrabber->isGrabbing()){
            QPixmap img = QPixmap::fromImage(ui->graphicsViewImage->imageItem->getImage());
            imageSet.append(img);
            ui->labelLastImage->setPixmap(img);
            ui->labelLastImage->setScaledContents(true);
            ui->pushButton->setText("Capture " + QString::number(imgCount++));
        }
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


void ImageCapture::on_pushButton_2_clicked()
{
    robot.publish("run");
    connect(&robot, SIGNAL(snapIt()),this,SLOT(snapIt()));
    connect(&robot,SIGNAL(robotSaid(QString)),this,SLOT(fromRobot(QString)));
    imgCount = 0;
}

void ImageCapture::snapIt()
{
    //QPixmap snap(QPixmap::fromImage(ui->graphicsViewImage->imageItem->getImage()));
    //vector<Mat> temp;
    //QThread::sleep(1);
    try {
        QPixmap t = QPixmap::fromImage(ui->graphicsViewImage->imageItem->getImage());

        //QString path = QDir::currentPath() + "/BOW/RecentSet/" + QString::number(imgCount) + ".jpg";

       // QString path = QDir::currentPath() + "/BOW/RecentSet/" + time.currentDateTime().toString(Qt::ISODate) + ".jpg";
       // t.save(path,"JPG");
       // temp = convert(ui->graphicsViewImage->imageItem->getImage());
        testImagesMap[imgCount++] = t;
        ui->labelLastImage->setPixmap(t);
        ui->labelLastImage->setScaledContents(true);
    } catch (Exception e) {
        qDebug() << e.code;
    }
    //testImages.insert(testImages.end(),temp.begin(),temp.end());
    //QThread::sleep(1);
    robot.publish("run");
    qDebug() << "Asked to Run";
}

void ImageCapture::fromRobot(QString m)
{
    QMessageBox msg;
    if(m == "ud"){
        robot.publish("hm");
        qDebug() << "Asked to go home";
        msg.setWindowTitle("Finished");
        msg.setText("Autorun Finished");
        msg.setIcon(QMessageBox::Information);
        msg.exec();

        if(testImagesMap.size() > 0){
            QDir dir = (QDir::currentPath()+"/BOW/Recent/A");
            if(dir.exists()){
                QDir odir = QDir::currentPath()+ "/BOW/Old";
                if(!odir.exists()) odir.mkpath(odir.path());

                dir.rename(dir.path(),(QDir::currentPath() + "/BOW/Old/" +  QDateTime::currentDateTime().toString(Qt::ISODate)));
            }

            dir.mkpath(dir.path());
            for(map<int,QPixmap>::iterator it= testImagesMap.begin();it!=testImagesMap.end();it++){
                it->second.save(dir.path()+"/"+ QString::number(it->first)+".jpg","JPG");
            }

            BOW *b = new BOW();
            b->initVocabulary(1);
            b->initSVMs(1);

            QMultiMap<int,vector<pair<string,float>>> tmpres = b->testFolder(QUrl(QDir::currentPath()+"/BOW/Recent"),1);
            b->saveXMLFile("Recent/A/" + QDateTime::currentDateTime().toString(Qt::ISODate),tmpres);

            multimap<int,QPixmap> res;
            for(QMultiMap<int,vector<pair<string,float > > >::iterator it=tmpres.begin();it!=tmpres.end();it++){
                vector<pair<string,float>>::iterator itv = it.value().begin();
                //qDebug() << QDir::currentPath()+"/BOW/images/"+ QString::fromStdString((*itv).first) + "/10.jpg";
                QPixmap mp(QDir::currentPath()+"/BOW/images/"+ QString::fromStdString((*itv).first) + "/10.jpg");
                qDebug() << QString::fromStdString((*itv).first).toInt();
                res.insert(pair<int,QPixmap>(QString::fromStdString((*itv).first).toInt(),mp));
            }
            //emit call_grid(res);
            multimap<int,QPixmap> *m = &res;
        //    //qDebug() << "Signal Emited to get the display";
            qDebug() << m->size();

            display *dis= new display(0,m);
            dis->show();

        }
    }
}

void ImageCapture::on_btnBack_2_clicked()
{
    this->close();
}
