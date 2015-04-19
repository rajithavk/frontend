#ifndef ADDITEMS_H
#define ADDITEMS_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class addItems;
}

class addItems : public QDialog
{
    Q_OBJECT

public:
    explicit addItems(QWidget *parent = 0, int rowid=0, QString image_path=0);
    ~addItems();
    void set_lineEdit_name ( const QString & text );
    void set_label_image ( const QString & text );
    void set_lineEdit_order ( const QString & text );
    void set_lineEdit_price ( const QString & text );
    void set_lineEdit_code ( const QString & text );
    void set_lineEdit_other ( const QString & text );

private slots:
    void on_save_clicked();
    void on_browse_image_clicked();
    void on_Exit_clicked();

private:
    Ui::addItems *ui;
};

#endif // ADDITEMS_H
