#ifndef IMAGELISTMODEL_H
#define IMAGELISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QPixmap>
#include <QDir>
#include <iostream>
#include <QKeyEvent>
using namespace std;


class imagelistmodel : public QAbstractListModel
{
    Q_OBJECT
public:

    explicit imagelistmodel(QVector<QPair<QString,QPixmap>> ims,QObject *parent=0);
    virtual ~imagelistmodel();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE bool removeRow(int row);

private:
    QVector<QPair<QString,QPixmap>> _data;
};

#endif // IMAGELISTMODEL_H
