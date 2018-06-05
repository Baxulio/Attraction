#include "CustomQuickImageProvider.h"
#include <QDebug>

CustomQuickImageProvider::CustomQuickImageProvider():
    QQuickImageProvider (QQuickImageProvider::Image)
{
}

void CustomQuickImageProvider::setTableModel(QSqlRelationalTableModel &model)
{
    this->tableModel = &model;
}

QImage CustomQuickImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage tempImage;
    tempImage.loadFromData(tableModel->data(tableModel->index(id.toInt(),tableModel->fieldIndex("icon")), Qt::EditRole).toByteArray(), "PNG");

    size->setHeight(tempImage.height());
    size->setWidth(tempImage.width());

    tempImage.scaled(requestedSize, Qt::KeepAspectRatio);

    return tempImage;
}
