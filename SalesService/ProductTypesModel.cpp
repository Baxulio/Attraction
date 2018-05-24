#include "ProductTypesModel.h"

ProductTypesModel::ProductTypesModel(QObject *parent)
{
    setTable("product_types");
    setSort(1, Qt::DescendingOrder);
    // Ensures that the model is sorted correctly after submitting a new row.
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}
