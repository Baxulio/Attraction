#include "ProductsProxyModel.h"

#include <QSqlTableModel>
#include <QSqlRecord>

#include <QPixmap>

ProductsProxyModel::ProductsProxyModel(QObject *parent):
    QSortFilterProxyModel(parent),
    b_type_id(0){}

void ProductsProxyModel::setTypeIdFilter(const int &type_id)
{
    b_type_id=type_id;
    invalidateFilter();
}

QVariant ProductsProxyModel::data(const QModelIndex &index, int role) const
{
    QSqlTableModel *model = qobject_cast<QSqlTableModel*>(sourceModel());
    if(role == Qt::DecorationRole){
        if(model->fieldIndex("title")==index.column()){
            QPixmap pix;
            pix.loadFromData(this->data(this->index(index.row(),model->fieldIndex("icon")),Qt::EditRole).toByteArray(),"PNG");
            return pix;
        }
    }
    else if(role == Qt::DisplayRole){
        return  this->data(this->index(index.row(),model->fieldIndex("title")),Qt::EditRole).toString()
                +"\n"
                +"["
                +this->data(this->index(index.row(),model->fieldIndex("amount")),Qt::EditRole).toString()
                +"] "
                +this->data(this->index(index.row(),model->fieldIndex("price")),Qt::EditRole).toString();
    }
    else if(role == Qt::ToolTipRole){
        return this->data(this->index(index.row(),model->fieldIndex("comment")),Qt::EditRole).toString();
    }
    return QSortFilterProxyModel::data(index,role);
}

bool ProductsProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent)
    return reinterpret_cast<QSqlTableModel*>(sourceModel())->record(source_row).value("product_types_id").toInt()==b_type_id;
}
