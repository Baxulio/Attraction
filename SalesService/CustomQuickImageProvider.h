#ifndef CUSTOMQUICKIMAGEPROVIDER_H
#define CUSTOMQUICKIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QSqlRelationalTableModel>

class CustomQuickImageProvider : public QQuickImageProvider
{
public:
    CustomQuickImageProvider();
    void setTableModel(QSqlRelationalTableModel &model);

    // QQuickImageProvider interface
public:
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    QSqlRelationalTableModel *tableModel;
};

#endif // CUSTOMQUICKIMAGEPROVIDER_H
