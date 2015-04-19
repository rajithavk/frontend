#ifndef DISPLAY_H
#define DISPLAY_H

#include "item_details.h"
#include "qrightclickpushbutton.h"
#include "selectgrid.h"

#include <QMainWindow>
#include <QMultiMap>
#include <QSqlDatabase>
#include <QSignalMapper>
#include <QMenu>

using namespace std;
namespace Ui {
class display;
}
namespace results_grid{
class display : public QMainWindow{
    Q_OBJECT

public:
    explicit display(QWidget *parent = 0,
                     QMultiMap<int, vector<pair<string, float> > > *pics=0);
    ~display();

private slots:
    void butgrid(const QString &passed);
    void select_grid(const QString &passed);
    void Delete(const QString &passed);
    void Replace(const QString &passed);

private:
    Ui::display *ui;
    QSqlDatabase dbc;
    QSignalMapper *signalMapper_click;
    QSignalMapper *signalMapper_rightclick;
    QSignalMapper *signalMapper_Edit;
    QSignalMapper *signalMapper_Delete;
    QSignalMapper *signalMapper_Replace;
    QMultiMap <int, QPixmap> *pics;
    QMultiMap<int, vector<pair<string, float> > > *pics_;
    item_details *details;
//    selectgrid slg;
};
};
#endif // DISPLAY_H
