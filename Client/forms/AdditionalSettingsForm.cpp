#include "AdditionalSettingsForm.h"
#include "ui_AdditionalSettingsForm.h"

AdditionalSettingsForm::AdditionalSettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdditionalSettingsForm),
    bDb(DatabaseManager::instance()),
    tariffs_model(new CustomSqlTableModel(this))
{
    ui->setupUi(this);

    tariffs_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tariffs_table->setModel(tariffs_model);

    connect(&bDb, &DatabaseManager::refresh, this, &AdditionalSettingsForm::on_refresh);
    on_refresh();
}

AdditionalSettingsForm::~AdditionalSettingsForm()
{
    delete ui;
}

void AdditionalSettingsForm::on_refresh()
{
    tariffs_model->setTable("tariff");
    if(!tariffs_model->select()){
        bDb.debugError(tariffs_model->lastError());
        return ;
    }
    ui->tariffs_table->hideColumn(tariffs_model->fieldIndex("id"));

    tariffs_model->setHeaderData(tariffs_model->fieldIndex("title"),Qt::Horizontal,"Название");
    tariffs_model->setHeaderData(tariffs_model->fieldIndex("price"),Qt::Horizontal,"Цена");
    tariffs_model->setHeaderData(tariffs_model->fieldIndex("time_limit"),Qt::Horizontal,"Время [в минутах]");
}

void AdditionalSettingsForm::on_add_tariff_clicked()
{
    tariffs_model->insertRow(tariffs_model->rowCount());
}

void AdditionalSettingsForm::on_delete_tariff_clicked()
{
    tariffs_model->removeRow(ui->tariffs_table->currentIndex().row());
}

void AdditionalSettingsForm::on_tariffs_submit_clicked()
{
    if(!tariffs_model->submitAll())
        bDb.debugError(tariffs_model->lastError());
}

void AdditionalSettingsForm::on_tariffs_revert_clicked()
{
    tariffs_model->revertAll();
}

void AdditionalSettingsForm::on_back_but_clicked()
{
    emit back();
}
