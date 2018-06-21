#ifndef PRODUCTSPROXYMODEL_H
#define PRODUCTSPROXYMODEL_H

#include <QSortFilterProxyModel>

class ProductsProxyModel : public QSortFilterProxyModel
{
public:
    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
};

#endif // PRODUCTSPROXYMODEL_H
