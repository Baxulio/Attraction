#ifndef SALESMODEL_H
#define SALESMODEL_H

#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QSqlRecord>

class SalesModel : public QSqlTableModel
{
    Q_OBJECT

public:
    SalesModel(QObject *parent = 0);

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
};

class CartProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override {
        SalesModel *model = const_cast<SalesModel*>(qobject_cast<const SalesModel*>(source_parent.model()));
        if(model && model->record(source_row).value("amount").toInt()>0)
            return true;
        return false;
    }

public:
    void invalidate(){
        invalidateFilter();
    }
};

#endif // SALESMODEL_H
