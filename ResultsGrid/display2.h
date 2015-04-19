#ifndef DISPLAY2_H
#define DISPLAY2_H

#include "grid/item_details.h"
#include "grid/qrightclickpushbutton.h"
#include "selectgrid.h"

#include <QMainWindow>
#include <QMultiMap>
#include <QSqlDatabase>
#include <QSignalMapper>
#include <QMenu>
using namespace std;
namespace Ui {
class display2;
}

class display2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit display2(QWidget *parent = 0,
                      QMultiMap<int, vector<pair<string, float> > > *pics=0);
    ~display2();

private slots:
    void butgrid(const QString &passed);
    void select_grid(const QString &passed);
    void Delete(const QString &passed);
    void Replace(const QString &passed);

private:
    Ui::display2 *ui;
    QSqlDatabase dbc;
    QSignalMapper *signalMapper_click;
    QSignalMapper *signalMapper_rightclick;
    QSignalMapper *signalMapper_Edit;
    QSignalMapper *signalMapper_Delete;
    QSignalMapper *signalMapper_Replace;
    QMultiMap <int, QPixmap> *pics;
    QMultiMap<int, vector<pair<string, float> > > *pics_;
    item_details *details;

};

#endif // DISPLAY2_H
