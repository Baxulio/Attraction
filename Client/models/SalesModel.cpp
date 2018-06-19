#include "SalesModel.h"

#include <QPixmap>

SalesModel::SalesModel(QObject *parent) :
    QSqlTableModel(parent)
{
}

QVariant SalesModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DecorationRole){
        QSqlRecord rec = const_cast<SalesModel*>(qobject_cast<const SalesModel*>(index.model()))->record(index.row());
        if(rec.fieldName(index.column())=="title"){
            QPixmap pix;
            pix.loadFromData(rec.value("icon").toByteArray(),"PNG");
            return pix;
        }
    }
    else if(role == Qt::DisplayRole){
        QSqlRecord rec = const_cast<SalesModel*>(qobject_cast<const SalesModel*>(index.model()))->record(index.row());
        if(rec.fieldName(index.column())=="title" && rec.value("amount").isValid()){
            return QSqlTableModel::data(index).toString()+"\n"+"["+rec.value("amount").toString()+"] "+rec.value("price").toString();
        }
    }
    else if(role == Qt::ToolTipRole){
        return const_cast<SalesModel*>(qobject_cast<const SalesModel*>(index.model()))->record(index.row()).value("comment").toString();
    }

    return QSqlTableModel::data(index,role);
}
