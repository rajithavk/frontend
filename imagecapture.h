#ifndef IMAGECAPTURE_H
#define IMAGECAPTURE_H

#include <QWidget>
#include <QSettings>
#include <QCloseEvent>


#include "qimagegrabber/qimagegrabber.h"
#include "qimagegrabber/qimagegrabberhttp.h"
#include "qimagegrabber/qimagegrabbermjpeg.h"
#include "qimagegrabber/dialogqimagegrabbersettings.h"
#include  "robot/rosclient.h"
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include "robot/crop.h"

using namespace cv;
using namespace std;

namespace Ui {
class ImageCapture;
}

class ImageCapture : public QWidget
{
    Q_OBJECT

public:
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

    vector<Mat> testImages;

signals:
    void gotImageSet(QVector<QPixmap>);

private slots:

    void on_comboBoxGrabberTypes_activated(int index);

    void newImageReceived(QImage *);
    void newFPSCalculated(double,double);
    void grabbingStateChanged(QImageGrabber::GrabbingState newState);

    void on_pushButtonStart_clicked();
    void on_pushButtonSettings_clicked();
    void on_pushButton_clicked();
    void on_btnConnect_clicked();
    void on_btnRight_clicked();
    void on_btnForward_clicked();
    void on_btnLeft_clicked();
    void on_btnBack_clicked();
    void on_btnStop_clicked();

    void gotConState(QString state);
    void on_pushButton_2_clicked();

    void snapIt();
};

#endif // IMAGECAPTURE_H
