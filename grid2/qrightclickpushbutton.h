#ifndef QRIGHTCLICKPUSHBUTTON_H
#define QRIGHTCLICKPUSHBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class QRightClickPushButton : public QPushButton{

//public:
//    QRightClickPushButton();
    Q_OBJECT

public:
    explicit QRightClickPushButton(QWidget *parent = 0);

//private slots:
//    void mousePressEvent(QMouseEvent *e);

signals:
    void rightClicked();
    void clicked();

protected:
    void mouseReleaseEvent(QMouseEvent *e) {
        if (e->button() == Qt::RightButton) emit clicked();
        else if (e->button() == Qt::LeftButton) emit rightClicked();
    }
};

#endif // QRIGHTCLICKPUSHBUTTON_H
