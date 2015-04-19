#ifndef DISPLAY_H
#define DISPLAY_H

#include "item_details.h"
#include "qrightclickpushbutton.h"
#include  "inventory.h"
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
                     multimap<int, QPixmap> *pics=0);
    ~display();

private slots:
    void butgrid(const QString &passed);
    void Delete(const QString &passed);
    void Replace(const QString &passed);
    void Edit(const QString &passed);

private:
    Ui::display *ui;
    QSqlDatabase dbc;
    QSignalMapper *signalMapper_click;
    QSignalMapper *signalMapper_rightclick;
    QSignalMapper *signalMapper_Edit;
    QSignalMapper *signalMapper_Delete;
    QSignalMapper *signalMapper_Replace;
    multimap <int, QPixmap> *pics;
    item_details *details;



};

#endif // DISPLAY_H
