#ifndef PRODUCTTYPESMODEL_H
#define PRODUCTTYPESMODEL_H

#include <QSqlTableModel>

class ProductTypesModel : public QSqlTableModel
{
    Q_OBJECT

public:
    ProductTypesModel(QObject *parent = 0);
};

#endif // PRODUCTTYPESMODEL_H
