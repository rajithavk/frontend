#ifndef SELECTGRID_H
#define SELECTGRID_H

#include <QMainWindow>

namespace Ui {
class selectgrid;
}

class selectgrid : public QMainWindow
{
    Q_OBJECT

public:
    explicit selectgrid(QWidget *parent = 0,QString mainIm = 0, QString sIm1 = 0, QString sIm2 = 0, QString sIm3 = 0,
            float fIm1 = 0, float fIm2 = 0, float fIm3 = 0);
    ~selectgrid();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::selectgrid *ui;
    QString mainIm,sIm1,sIm2,sIm3;
    float fIm1,fIm2,fIm3;

};

#endif // SELECTGRID_H
