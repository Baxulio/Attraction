#include "TableModel.h"
#include <QSqlRecord>

TableModel::TableModel(QObject *parent) :
    QSqlRelationalTableModel (parent)
{
    setTable("product_types");
    setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
}

TableModel::~TableModel()
{
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if(index.row() >= rowCount())
    {
        return QString("");
    }
    if(role < Qt::UserRole)
    {
        return QSqlRelationalTableModel::data(index, role);
    }
    else
    {
        // search for relationships
        for (int i = 0; i < columnCount(); i++)
        {
            if (this->relation(i).isValid())
            {
                return record(index.row()).value(QString(roles.value(role)));
            }
        }

        // if no valid relationship was found
        return QSqlRelationalTableModel::data(this->index(index.row(), role - Qt::UserRole - 1), Qt::DisplayRole);
    }
}

void TableModel::generateRoleNames()
{
    roles.clear();
    int nbCols = this->columnCount();

    for (int i = 0; i < nbCols; i++)
    {
        roles[Qt::UserRole + i + 1] = this->headerData(i, Qt::Horizontal).toByteArray();
    }
}
