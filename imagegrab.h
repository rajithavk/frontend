#ifndef IMAGEGRAB_H
#define IMAGEGRAB_H

#include <QWidget>

namespace Ui {
class imagegrab;
}

class Imagegrab : public QWidget
{
    Q_OBJECT

public:
    explicit Imagegrab(QWidget *parent = 0);
    ~Imagegrab();

private:
    Ui::imagegrab *ui;
};

#endif // IMAGEGRAB_H
