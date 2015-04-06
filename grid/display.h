#ifndef DISPLAY_H
#define DISPLAY_H

#include "item_details.h"
#include "qrightclickpushbutton.h"

#include <QMainWindow>
#include <map>
#include <QSqlDatabase>
#include <QSignalMapper>
#include <QMenu>

using namespace std;
namespace Ui {
class display;
}

class display : public QMainWindow{
    Q_OBJECT

public:
    explicit display(QWidget *parent = 0,
                     map<int, QPixmap> *pics=0);
    ~display();

private slots:
    void butgrid(const QString &passed);

private:
    Ui::display *ui;
    QSqlDatabase dbc;
    QSignalMapper *signalMapper_click;
    QSignalMapper *signalMapper_rightclick;
    map <int, QPixmap> *pics;
    item_details *details;

};

#endif // DISPLAY_H
