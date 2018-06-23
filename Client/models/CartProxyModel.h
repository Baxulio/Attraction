#ifndef CARTPROXYMODEL_H
#define CARTPROXYMODEL_H

#include <QSortFilterProxyModel>

class CartProxyModel : public QSortFilterProxyModel
{
public:
    void invalidate(){
        invalidateFilter();
    }
    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // CARTPROXYMODEL_H
