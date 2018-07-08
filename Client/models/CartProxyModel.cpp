#include "CartProxyModel.h"

#include <QSqlTableModel>
#include <QSqlRecord>

QVariant CartProxyModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole){
        QString temp = sourceModel()->headerData(index.column(),Qt::Horizontal,Qt::EditRole).toString();
        int amount = QSortFilterProxyModel::data(this->index(index.row(), reinterpret_cast<QSqlTableModel*>(sourceModel())->fieldIndex("amount")),Qt::EditRole).toInt();

        if(temp == "title")
            return QString("%1 x %2").arg(amount).arg(QSortFilterProxyModel::data(index,Qt::EditRole).toString());
        if(temp == "price")
            return amount*QSortFilterProxyModel::data(this->index(index.row(), reinterpret_cast<QSqlTableModel*>(sourceModel())->fieldIndex("price")),Qt::EditRole).toDouble();
    }
    return QSortFilterProxyModel::data(index,role);
}

bool CartProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent)
    return reinterpret_cast<QSqlTableModel*>(sourceModel())->record(source_row).value("amount").toInt()>0;
}
