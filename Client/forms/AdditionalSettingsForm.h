#ifndef ADDITIONALSETTINGSFORM_H
#define ADDITIONALSETTINGSFORM_H

#include <QWidget>
#include "DatabaseManager.h"
#include "models/CustomSqlTableModel.h"

namespace Ui {
class AdditionalSettingsForm;
}

class AdditionalSettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit AdditionalSettingsForm(QWidget *parent = 0);
    ~AdditionalSettingsForm();

private slots:
    void on_refresh();

    void on_add_tariff_clicked();

    void on_delete_tariff_clicked();

    void on_tariffs_submit_clicked();

    void on_tariffs_revert_clicked();

    void on_back_but_clicked();

private:
    Ui::AdditionalSettingsForm *ui;
    DatabaseManager &bDb;
    CustomSqlTableModel *tariffs_model;

signals:
    void back();
};

#endif // ADDITIONALSETTINGSFORM_H
