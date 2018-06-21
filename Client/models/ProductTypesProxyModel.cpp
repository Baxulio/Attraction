#include "ProductTypesProxyModel.h"

#include <QSqlTableModel>
#include <QSqlRecord>

#include <QPixmap>

QVariant ProductTypesProxyModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DecorationRole){
        QSqlRecord rec = reinterpret_cast<QSqlTableModel*>(sourceModel())->record(index.row());
        if(rec.fieldName(index.column())=="title"){
            QPixmap pix;
            pix.loadFromData(rec.value("icon").toByteArray(),"PNG");
            return pix;
        }
    }
    else if(role == Qt::ToolTipRole){
        return reinterpret_cast<QSqlTableModel*>(sourceModel())->record(index.row()).value("comment").toString();
    }
    return QSortFilterProxyModel::data(index,role);
}
