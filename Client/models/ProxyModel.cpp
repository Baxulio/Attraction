#include "ProxyModel.h"
#include <QSqlTableModel>

ProxyModel::ProxyModel(QObject *parent):
    QSortFilterProxyModel(parent),
    bracer_number(0),
    enter_number(0),
    tariff_id(0),
    //history
    exit_number(0)
  //
{
}

void ProxyModel::invalidate()
{
    invalidateFilter();
}

bool ProxyModel::dateInRange(const QDateTime &dateTime, bool mode) const
{
    if(mode){
        return (!in_time_from.isValid() || dateTime>=in_time_from)
                && (!in_time_to.isValid() || dateTime<=in_time_to);
    }
    return (!out_time_from.isValid() || dateTime>=out_time_from)
            && (!out_time_to.isValid() || dateTime<=out_time_to);
}

bool ProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(tariff_id && sourceModel()->data(sourceModel()->index(source_row, static_cast<QSqlTableModel*>(sourceModel())->fieldIndex("tariff_id"), source_parent)).toInt()!=tariff_id)
        return false;

    if(enter_number && sourceModel()->data(sourceModel()->index(source_row, static_cast<QSqlTableModel*>(sourceModel())->fieldIndex("enter_number"), source_parent)).toInt()!=enter_number)
        return false;

    if(exit_number && sourceModel()->data(sourceModel()->index(source_row, static_cast<QSqlTableModel*>(sourceModel())->fieldIndex("exit_number"), source_parent)).toInt()!=exit_number)
        return false;

    if(bracer_number && sourceModel()->data(sourceModel()->index(source_row, static_cast<QSqlTableModel*>(sourceModel())->fieldIndex("bracer_number"), source_parent)).toInt()!=bracer_number)
        return false;

    if(!dateInRange(sourceModel()->data(sourceModel()->index(source_row, static_cast<QSqlTableModel*>(sourceModel())->fieldIndex("enter_time"), source_parent)).toDateTime(),1))
        return false;

    if(!dateInRange(sourceModel()->data(sourceModel()->index(source_row, static_cast<QSqlTableModel*>(sourceModel())->fieldIndex("exit_time"), source_parent)).toDateTime(),0))
        return false;

    return true;
}

bool ProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{

    QString fieldString = sourceModel()->headerData(source_column,Qt::Horizontal,Qt::DisplayRole).toString();
    if(fieldString=="id" or fieldString=="code" or fieldString=="deposit_id" or fieldString=="tariff_id" or fieldString=="cash")
        return false;
    return  true;
}

QVariant ProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal and role == Qt::DisplayRole){
        if(QSortFilterProxyModel::headerData(section,Qt::Horizontal,Qt::DisplayRole).toString()=="bracer_number")return "Номер браслета";
        if(QSortFilterProxyModel::headerData(section,Qt::Horizontal,Qt::DisplayRole).toString()=="enter_time")return "Время входа";
        if(QSortFilterProxyModel::headerData(section,Qt::Horizontal,Qt::DisplayRole).toString()=="enter_number")return "Номер входа";
        if(QSortFilterProxyModel::headerData(section,Qt::Horizontal,Qt::DisplayRole).toString()=="exit_time")return "Время выхода";
        if(QSortFilterProxyModel::headerData(section,Qt::Horizontal,Qt::DisplayRole).toString()=="exit_number")return "Номер выхода";
    }
    return QSortFilterProxyModel::headerData(section, orientation,role);
}
