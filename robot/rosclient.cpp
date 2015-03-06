#include "rosclient.h"

ROSClient::ROSClient(QObject *parent) :
  QObject(parent)
{
    connect(&ws,SIGNAL(connected()),this,SLOT(on_connected()));
    connect(&ws,SIGNAL(textMessageReceived(QString)),this,SLOT(on_message(QString)));
    connect(&ws,SIGNAL(disconnected()),this,SLOT(on_disonnect()));
    connected = false;

}

ROSClient::~ROSClient(){

}

void ROSClient::publish(QString message){

    json::Object msg;
    msg["data"] = message.toStdString();

    json::Object pubObj;
    pubObj["op"]="publish";
    pubObj["topic"] = "user_cmnds";
    pubObj["msg"]=msg;

    ws.sendTextMessage(QString::fromStdString(json::Serialize(pubObj)));

}

void ROSClient::open(QString rURL)
{
    if(!connected)
        ws.open(QUrl(rURL));
    else
        ws.close();
}

void ROSClient::on_message(QString message){
    QString response;
    json::Object my_object = json::Deserialize(message.toStdString());

    json::Object::ValueMap::iterator it = my_object.find("msg");
    json::Object::ValueMap::iterator it1;

    if (it != my_object.end())
    {
      json::Object msgObj = it->second;
      it1 = msgObj.find("data");
    }
    response=QString::fromStdString(it1->second.ToString());
    qDebug() << "Robot Said : " << response;
    if(response=="pd"){
        emit(snapIt());
    }
    else if(response=="md" || response=="td" || response=="ed" || response=="id"){
        emit(robotSaid(response));
    }
}

void ROSClient::on_connected(){
    qDebug() << "Websocket Connected";
    connected = true;
    json::Object advObj;
    advObj["op"]="advertise";
    advObj["topic"]="user_cmnds";
    advObj["type"]= "std_msgs/String";

    ws.sendTextMessage(QString::fromStdString(json::Serialize(advObj)));

    qDebug() << "Advertised";

    json::Object subObj;
    subObj["op"]="subscribe";
    subObj["topic"]="response";
    subObj["type"]= "std_msgs/String";
    ws.sendTextMessage(QString::fromStdString(json::Serialize(subObj)));

    qDebug() << "Subscribed";
    emit(conState("Connected"));
}

void ROSClient::on_disonnect()
{
    qDebug() << "Websocket Disconnected";
    connected = false;
    emit(conState("Disconnected"));

}

