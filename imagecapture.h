#ifndef IMAGECAPTURE_H
#define IMAGECAPTURE_H

#include <QWidget>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDir>
#include <QThread>
#include <QDateTime>
#include "qimagegrabber/qimagegrabber.h"
#include "qimagegrabber/qimagegrabberhttp.h"
#include "qimagegrabber/qimagegrabbermjpeg.h"
#include "qimagegrabber/dialogqimagegrabbersettings.h"
#include  "robot/rosclient.h"
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include "robot/crop.h"
#include "grid/display.h"
#include <opencv2/opencv.hpp>
#include "CONV.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "global_objects.h"

using namespace cv;
using namespace std;

namespace Ui {
class ImageCapture;
}

class ImageCapture : public QWidget
{
    Q_OBJECT


public:
    //explicit ImageCapture(QWidget *parent = 0, cv::VideoCapture *c = NULL);
    explicit ImageCapture(QWidget *parent = 0);
    ~ImageCapture();
     void closeEvent(QCloseEvent *);


private:
    Ui::ImageCapture *ui;
    QImageGrabberMjpeg *mjpgGrabber;
    QList<QImageGrabber*> imageGrabbers;
    QImageGrabber *currentGrabber;
    DialogQImageGrabberSettings settingsDialog;
    QSettings settings;
    bool firstImageRecieved;
    QVector<QPixmap> imageSet;
    void setImageGrabber(QImageGrabber *gb);
    ROSClient robot;
    map<int,QPixmap> testImagesMap;
    int imgCount;
    QDateTime time;

    cv::VideoCapture vcap;
    cv::Mat image;
    bool cvGrabber;
    bool cvIsGrabbing;
    void opencvGrabImage();
    QString URL;
    QImage *cvImage;

signals:
    void gotImageSet(QVector<QPixmap>);
    void newOpencvGrabbedImage(QImage *);

private slots:

    void on_comboBoxGrabberTypes_activated(int index);

    void newImageReceived(QImage *);
    void newFPSCalculated(double,double);
    void grabbingStateChanged(QImageGrabber::GrabbingState newState);
    void connectionState(QString m);

    void on_pushButtonStart_clicked();
    void on_pushButtonSettings_clicked();
    void on_pushButton_clicked();
    void on_btnConnect_clicked();
    void on_btnRight_clicked();
    void on_btnForward_clicked();
    void on_btnLeft_clicked();
    void on_btnBack_clicked();
    void on_btnStop_clicked();

    void on_pushButton_2_clicked();

    void snapIt();
    void fromRobot(QString m);
};

#endif // IMAGECAPTURE_H
