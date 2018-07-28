#ifndef BARSTATUSFORM_H
#define BARSTATUSFORM_H

#include <QWidget>
#include "delegates/TransactionsFrame.h"
#include <dialogs/SettingsDialog.h>

namespace Ui {
class BarStatusForm;
}

class BarStatusForm : public QWidget
{
    Q_OBJECT

public:
    explicit BarStatusForm(SettingsDialog &set, QWidget *parent = 0);
    ~BarStatusForm();

private:
    Ui::BarStatusForm *ui;
    TransactionsFrame *transactionsFrame;
    DatabaseManager &bDb;
    SettingsDialog &bSetings;
    QSqlRecord currentRecord;

private:
    bool retrieve_info(const quint32 &code);

signals:
    void back();
private slots:
    void on_back_but_clicked();
    void on_lineEdit_returnPressed();
};

#endif // BARSTATUSFORM_H
