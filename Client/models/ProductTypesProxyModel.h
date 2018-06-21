#ifndef PRODUCTTYPESPROXYMODEL_H
#define PRODUCTTYPESPROXYMODEL_H

#include <QSortFilterProxyModel>

class ProductTypesProxyModel : public QSortFilterProxyModel
{
public:
    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
};

#endif // PRODUCTTYPESPROXYMODEL_H
