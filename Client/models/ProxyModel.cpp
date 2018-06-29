#include "ProxyModel.h"
#include <QSqlTableModel>

#include "DatabaseManager.h"
#include <QSqlRecord>
#include <QBrush>
#include <QColor>
#include <QDebug>
ProxyModel::ProxyModel(QObject *parent):
    QSortFilterProxyModel(parent),
    bracer_number(0),
    enter_number(0),
    childs_number(0),
    //history
    exit_number(0),
  //
    timeout(false),
    warning(false)
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
    if(childs_number && sourceModel()->data(sourceModel()->index(source_row, static_cast<QSqlTableModel*>(sourceModel())->fieldIndex("childs"), source_parent)).toInt()!=childs_number)
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
    Q_UNUSED(source_parent)
    QString fieldString = sourceModel()->headerData(source_column,Qt::Horizontal,Qt::DisplayRole).toString();
    if(fieldString=="id" or fieldString=="code" or fieldString=="deposit_id" or fieldString=="cash" or fieldString=="childs")
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
        if(QSortFilterProxyModel::headerData(section,Qt::Horizontal,Qt::DisplayRole).toString()=="expected_exit_time")return "Ожидаемое время выхода";
    }
    return QSortFilterProxyModel::headerData(section, orientation,role);
}

QVariant ProxyModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::BackgroundColorRole){
        if(timeout and this->data(this->index(index.row(),3),Qt::EditRole).toDateTime() < QDateTime::currentDateTime())
            return QBrush(QColor(Qt::red));
        else if(warning and QDateTime::currentDateTime().secsTo(this->data(this->index(index.row(),3),Qt::EditRole).toDateTime())<1800)
            return QBrush(QColor(Qt::yellow));
    }
    return QSortFilterProxyModel::data(index,role);
}
