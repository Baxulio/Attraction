#include "SalesForm.h"
#include "ui_SalesForm.h"
#include <QItemSelectionModel>
#include <QSqlRecord>

SalesForm::SalesForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SalesForm),
    bDb(DatabaseManager::instance())
{
    ui->setupUi(this);

    productsProxyModel.setSourceModel(&productsModel);

    ui->types_listView->setModel(&typesModel);
    ui->products_listView->setModel(&productsProxyModel);

    connect(&bDb, &DatabaseManager::refresh, this, &SalesForm::on_refresh);
    connect(ui->types_listView->selectionModel(), &QItemSelectionModel::currentRowChanged, [this](QModelIndex cur, QModelIndex prev){
        Q_UNUSED(prev)
        QSqlRecord rec = typesModel.record(cur.row());
        productsProxyModel.setFilterFixedString(rec.value("id").toString());
        ui->product_type_label->setText(rec.value("title").toString().toUpper());
    });

    on_refresh();
}

SalesForm::~SalesForm()
{
    delete ui;
}

void SalesForm::on_refresh()
{
    typesModel.setTable("product_types");
    if(!typesModel.select()){
        bDb.debugError(typesModel.lastError());
        return ;
    }
    ui->types_listView->setModelColumn(typesModel.fieldIndex("title"));

    productsModel.setTable("products");
    if(!productsModel.select()){
        bDb.debugError(productsModel.lastError());
        return ;
    }
    ui->products_listView->setModelColumn(productsModel.fieldIndex("title"));
    productsProxyModel.setFilterKeyColumn(productsModel.fieldIndex("product_types_id"));
}

void SalesForm::on_back_but_clicked()
{
    emit back();
}

void SalesForm::on_products_listView_doubleClicked(const QModelIndex &index)
{

}
