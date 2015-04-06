#ifndef WELCOME_H
#define WELCOME_H

#include <QMainWindow>
#include "bow.h"
#include "inventory.h"
#include "imagecapture.h"
#include "grid/display.h"

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

private:
    Ui::welcome *ui;
    BOW *bow;
    ImageCapture *imgcap;
    Inventory *invt;
    display *d;

};

#endif // WELCOME_H
