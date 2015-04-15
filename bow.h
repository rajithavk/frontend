#ifndef BOW_H
#define BOW_H

#include <QMainWindow>
#include <iostream>
#include <imagelistmodel.h>
#include <QDebug>
#include <QXmlStreamWriter>
using namespace std;

namespace Ui {
class BOW;
}

class BOW : public QMainWindow
{
    Q_OBJECT

public:
    explicit BOW(QWidget *parent = 0);
    ~BOW();

    QMultiMap<QString,vector<pair<string,float>>> testFolder(QUrl url);
    void saveXMLFile(QString fileName,QMultiMap<QString,vector<pair<string,float>>> & results);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_comboBox_activated(const QString &arg1);

    void on_pushButton_6_clicked();

    void on_btnTestFolder_clicked();



private:
    Ui::BOW *ui;

};

#endif // BOW_H
