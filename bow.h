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

    QMultiMap<int,vector<pair<string,float>>> testFolder(QUrl url, bool mode);
    void saveXMLFile(QString fileName, QMultiMap<int, vector<pair<string, float> > > &results);
    void initVocabulary(bool s);
    void initSVMs(bool s);

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

    struct{
        bool operator()(QString& a,QString& b){
            qDebug() << a + " " + b;
            return a.toInt() < b.toInt();
         }
    } compareid;

};
#endif // BOW_H
