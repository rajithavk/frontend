#ifndef INVENTORY_H
#define INVENTORY_H

#include <QWidget>
#include <QDebug>
#include <QtSql/QtSql>
#include <QMessageBox>
#include <QFileDialog>

namespace Ui {
class inventory;
}

class inventory : public QWidget
{
    Q_OBJECT

public:
    explicit inventory(QWidget *parent = 0);
    ~inventory();
    inline bool isEmpty();
    inline void nextId();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

private:
    Ui::inventory *ui;
    QSqlDatabase db;
    QSqlQuery *qry;
    QSqlRecord rec;

};


#endif // INVENTORY_H
