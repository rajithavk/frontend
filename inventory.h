#ifndef INVENTORY_H
#define INVENTORY_H

#include <QWidget>

namespace Ui {
class inventory;
}

class inventory : public QWidget
{
    Q_OBJECT

public:
    explicit inventory(QWidget *parent = 0);
    ~inventory();

private:
    Ui::inventory *ui;
};

#endif // INVENTORY_H
