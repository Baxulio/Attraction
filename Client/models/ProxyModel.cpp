#include "ProxyModel.h"
#include <QSqlTableModel>

#include "DatabaseManager.h"
#include <QSqlRecord>
#include <QBrush>
#include <QColor>
#include <QDebug>

ProxyModel::ProxyModel(QObject *parent):
    QSortFilterProxyModel(parent),
    tariff_id(0),
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
    if(tariff_id && sourceModel()->data(sourceModel()->index(source_row, static_cast<QSqlTableModel*>(sourceModel())->fieldIndex("tariff_id"), source_parent)).toInt()!=tariff_id)
        return false;

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

QVariant ProxyModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::BackgroundColorRole){
        if(timeout and this->data(this->index(index.row(),static_cast<QSqlTableModel*>(sourceModel())->fieldIndex("expected_ecit_time")),Qt::EditRole).toDateTime() < QDateTime::currentDateTime())
            return QBrush(QColor(Qt::red));
        else if(warning and QDateTime::currentDateTime().secsTo(this->data(this->index(index.row(),static_cast<QSqlTableModel*>(sourceModel())->fieldIndex("expected_ecit_time")),Qt::EditRole).toDateTime())<1800)
            return QBrush(QColor(Qt::yellow));
    }
    return QSortFilterProxyModel::data(index,role);
}
