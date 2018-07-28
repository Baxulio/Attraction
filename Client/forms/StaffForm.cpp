#include "StaffForm.h"
#include "ui_StaffForm.h"

StaffForm::StaffForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StaffForm),
    bDb(DatabaseManager::instance()),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    ui->tableView->setModel(model);

    connect(&bDb, &DatabaseManager::refresh, this, &StaffForm::on_refresh);
    on_refresh();
}

StaffForm::~StaffForm()
{
    delete ui;
}

void StaffForm::on_refresh()
{
    model->setQuery("SELECT * FROM staff_traffic;");
    if (model->lastError().isValid()){
        bDb.debugError(model->lastError());
        return;
    }
}

void StaffForm::on_back_but_clicked()
{
    emit back();
}
