#ifndef WELCOME_H
#define WELCOME_H

#include <QMainWindow>
#include "bow.h"
#include "inventory.h"
#include "imagecapture.h"
#include "grid/display.h"
#include <global_objects.h>

namespace Ui {
class welcome;
}

class welcome : public QMainWindow
{
    Q_OBJECT

public:
    explicit welcome(QWidget *parent = 0);
    ~welcome();

private slots:
    void on_btnExit_clicked();

    void on_btnOps_clicked();

    void on_btnAdd_clicked();

    void on_btnRun_clicked();

    void on_btnGrid_clicked();

    void getRecordsForGrid(const QString &fileName);

    void on_btnStream_clicked();

    void on_call_for_inventory(map<int,QPixmap> res);

private:
    Ui::welcome *ui;
    BOW *bow;
    ImageCapture *imgcap;
    Inventory *invt;
    display *d;
    map<int,QPixmap> *mp;
     cv::VideoCapture vcap;
};

#endif // WELCOME_H
