#ifndef BRACERSFILTERDIALOG_H
#define BRACERSFILTERDIALOG_H

#include <QDialog>
#include "DatabaseManager.h"

namespace Ui {
class BracersFilterDialog;
}

class BracersFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BracersFilterDialog(bool b = true, QWidget *parent = 0);
    ~BracersFilterDialog();

    int tariff();
    int bracerNumber();
    int enterNumber();
    int childs();

    QDateTime enterTimeFrom();
    QDateTime enterTimeTo();

    //History
    int exitNumber();
    QDateTime exitTimeFrom();
    QDateTime exitTimeTo();
    //

private slots:
    void on_today_for_enter_but_clicked();
    void on_today_for_exit_but_clicked();

private:
    Ui::BracersFilterDialog *ui;
    DatabaseManager &bDb;
};

#endif // BRACERSFILTERDIALOG_H
