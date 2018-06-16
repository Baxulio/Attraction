#include "SalesModel.h"

#include <QSqlRecord>
#include <QPixmap>

SalesModel::SalesModel(QObject *parent) :
    QSqlTableModel(parent)
{
}

QVariant SalesModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DecorationRole){
        QSqlRecord rec = const_cast<SalesModel*>(qobject_cast<const SalesModel*>(index.model()))->record(index.row());

        QPixmap pix;
        pix.loadFromData(rec.value("icon").toByteArray(),
                         "PNG");
        return pix;
    }
    else if(role == Qt::ToolTipRole){
        QSqlRecord rec = const_cast<SalesModel*>(qobject_cast<const SalesModel*>(index.model()))->record(index.row());
        return QString(rec.value("comment").toString());
    }
    return QSqlTableModel::data(index,role);
}
