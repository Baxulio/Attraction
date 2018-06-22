#ifndef PRODUCTSPROXYMODEL_H
#define PRODUCTSPROXYMODEL_H

#include <QSortFilterProxyModel>

class ProductsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
private:
    int b_type_id;

public:
    ProductsProxyModel(QObject *parent = 0);

    void setTypeIdFilter(const int &type_id);
    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // PRODUCTSPROXYMODEL_H
