#include "imagelistmodel.h"

imagelistmodel::imagelistmodel(QVector<QPair<QString,QPixmap>> ims, QObject *parent)
    :QAbstractListModel(parent)
{
    _data = ims;
}


imagelistmodel::~imagelistmodel()
{
    _data.clear();
}

int imagelistmodel::rowCount(const QModelIndex &parent) const{
    if(parent.isValid())
        return 0;
    else
        return _data.count();
}


QVariant imagelistmodel::data(const QModelIndex &index, int role) const
{
    if(index.isValid()){
        switch (role) {
        case Qt::DecorationRole:
        {
            return _data.value(index.row()).second;
            break;
        }
        case Qt::DisplayRole:
        {
            return _data.value(index.row()).first;
            break;
        }
        default:
            break;
        }
    }
    return QVariant();
}


bool imagelistmodel::removeRow(int row){
    _data.removeAt(row);
    return 0;
}
