#ifndef STATUSFORM_H
#define STATUSFORM_H

#include <QWidget>
#include "delegates/TransactionsFrame.h"

namespace Ui {
class StatusForm;
}

class StatusForm : public QWidget
{
    Q_OBJECT

public:
    explicit StatusForm(QWidget *parent = 0);
    ~StatusForm();

private:
    Ui::StatusForm *ui;
    TransactionsFrame *transactionsFrame;
    DatabaseManager &bDb;
    QSqlRecord currentRecord;

public slots:
    void on_retrieve_info_but_clicked();

private slots:
    bool retrieve_info(const quint32 &code);
    void on_back_but_clicked();
    void on_return_debt_but_clicked();

signals:
    void back();
};

#endif // STATUSFORM_H
