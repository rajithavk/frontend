#ifndef INVENTORY_H
#define INVENTORY_H

#include <QWidget>
#include <QDebug>
#include <QtSql/QtSql>
#include <QMessageBox>
#include <QFileDialog>
#include "imagecapture.h"
#include <opencv2/opencv.hpp>

namespace Ui {
class inventory;
}

class Inventory : public QWidget
{
    Q_OBJECT

public:
    explicit Inventory(int id=0,QWidget *parent = 0);
    ~Inventory();
    inline bool isEmpty();
    inline void nextId();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();
    void on_imageset_recieve(QVector<QPixmap> );

    void on_pushButton_4_clicked();

private:
    Ui::inventory *ui;
    QSqlDatabase db;
    QSqlQuery *qry;
    QSqlRecord rec;
    ImageCapture *newCapture;
    QVector<QPixmap> imageSet;
    QUrl *defaultUrl;
    bool editingMode;
    int saveImageSet();
    cv::VideoCapture *capture;

signals :
    void exit();

};


#endif // INVENTORY_H
