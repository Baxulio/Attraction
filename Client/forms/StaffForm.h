#ifndef STAFFFORM_H
#define STAFFFORM_H

#include <QWidget>

#include "DatabaseManager.h"
#include <QSqlQueryModel>

namespace Ui {
class StaffForm;
}

class StaffForm : public QWidget
{
    Q_OBJECT

public:
    explicit StaffForm(QWidget *parent = 0);
    ~StaffForm();

private slots:
    void on_refresh();
    void on_back_but_clicked();

private:
    Ui::StaffForm *ui;
    DatabaseManager &bDb;
    QSqlQueryModel *model;

signals:
    void back();
};

#endif // STAFFFORM_H
