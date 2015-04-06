#ifndef ITEMS_DETAILS_H
#define ITEMS_DETAILS_H

#include <QMainWindow>

namespace Ui {
class items_details;
}

class items_details : public QMainWindow
{
    Q_OBJECT

public:
    explicit items_details(QWidget *parent = 0);
    ~items_details();

private:
    Ui::items_details *ui;
};

#endif // ITEMS_DETAILS_H
