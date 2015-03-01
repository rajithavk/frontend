#ifndef ROSCLIENT_H
#define ROSCLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include "json.h"
#include <QDebug>



class ROSClient   :   public QObject
{
    Q_OBJECT
public:
     ROSClient(QObject *parent=0);
    ~ROSClient();
     void open(QString rURL);
     void publish(QString message);

private:
     QWebSocket ws;
     bool connected;



signals:
    void conState(QString);
    void robotSaid(QString);
    void snapIt();

private slots:
     void on_message(QString message);
     void on_connected();
     void on_disonnect();
};

#endif // ROS

