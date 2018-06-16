#ifndef SALESMODEL_H
#define SALESMODEL_H

#include <QSqlTableModel>

class SalesModel : public QSqlTableModel
{
    Q_OBJECT

public:
    SalesModel(QObject *parent = 0);

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
};

#endif // SALESMODEL_H
