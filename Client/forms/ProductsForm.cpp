#include "ProductsForm.h"
#include "ui_ProductsForm.h"

#include <QItemSelectionModel>
#include <QSqlRecord>

#include <QPixmap>
#include <QFileDialog>
#include <QBuffer>
#include <QDebug>

ProductsForm::ProductsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductsForm),
    bDb(DatabaseManager::instance()),
    product_types_model(new CustomSqlTableModel(this)),
    products_model(new CustomSqlTableModel(this)),
    productsProxyModel(new QSortFilterProxyModel(this))
{
    ui->setupUi(this);

    product_types_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->product_types_table->setModel(product_types_model);

    products_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    productsProxyModel->setSourceModel(products_model);
    ui->products_table->setModel(productsProxyModel);

    connect(ui->product_types_table->selectionModel(), &QItemSelectionModel::currentRowChanged, [this](QModelIndex cur, QModelIndex prev){
        Q_UNUSED(prev)

        QPixmap pix;
        QByteArray arr = product_types_model->data(product_types_model->index(cur.row(), product_types_model->fieldIndex("icon")),Qt::EditRole).toByteArray();
        pix.loadFromData(arr,"PNG");

        ui->product_type_icon->setPixmap(pix);

        productsProxyModel->setFilterFixedString(product_types_model->data(product_types_model->index(cur.row(),product_types_model->fieldIndex("id")),Qt::DisplayRole).toString());
        ui->product_label->setText(product_types_model->data(product_types_model->index(cur.row(),product_types_model->fieldIndex("title")),Qt::DisplayRole).toString().toUpper());
    });
    connect(ui->products_table->selectionModel(), &QItemSelectionModel::currentRowChanged, [this](QModelIndex cur, QModelIndex prev){
        Q_UNUSED(prev)

        QPixmap pix;
        QByteArray arr = productsProxyModel->data(productsProxyModel->index(cur.row(), products_model->fieldIndex("icon")),Qt::EditRole).toByteArray();
        pix.loadFromData(arr,"PNG");

        ui->product_icon->setPixmap(pix);
    });
    connect(&bDb, &DatabaseManager::refresh, this, &ProductsForm::on_refresh);
    on_refresh();
}

ProductsForm::~ProductsForm()
{
    delete ui;
}

void ProductsForm::on_refresh()
{
    if(!this->isVisible())return;

    product_types_model->setTable("product_types");
    if(!product_types_model->select()){
        bDb.debugError(product_types_model->lastError());
        return ;
    }
    ui->product_types_table->hideColumn(product_types_model->fieldIndex("icon"));
    ui->product_types_table->hideColumn(product_types_model->fieldIndex("id"));

    product_types_model->setHeaderData(product_types_model->fieldIndex("title"),Qt::Horizontal,"Название");
    product_types_model->setHeaderData(product_types_model->fieldIndex("mode"),Qt::Horizontal,"Назначение");
    product_types_model->setHeaderData(product_types_model->fieldIndex("comment"),Qt::Horizontal,"Комментарий");

    products_model->setTable("products");
    if(!products_model->select()){
        bDb.debugError(products_model->lastError());
        return ;
    }

    products_model->setHeaderData(products_model->fieldIndex("title"),Qt::Horizontal,"Название");
    products_model->setHeaderData(products_model->fieldIndex("price"),Qt::Horizontal,"Цена");
    products_model->setHeaderData(products_model->fieldIndex("comment"),Qt::Horizontal,"Комментарий");    

    ui->products_table->hideColumn(products_model->fieldIndex("icon"));
    ui->products_table->hideColumn(products_model->fieldIndex("id"));
    ui->products_table->hideColumn(products_model->fieldIndex("product_types_id"));
    ui->products_table->hideColumn(products_model->fieldIndex("amount"));

    productsProxyModel->setFilterKeyColumn(products_model->fieldIndex("product_types_id"));

    ui->product_types_table->selectRow(0);
}

void ProductsForm::on_back_but_clicked()
{
    emit back();
}

void ProductsForm::on_types_revert_but_clicked()
{
    product_types_model->revertAll();
}

void ProductsForm::on_types_submit_but_clicked()
{
    if(!product_types_model->submitAll())
        bDb.debugError(product_types_model->lastError());
}

void ProductsForm::on_add_product_type_but_clicked()
{
    product_types_model->insertRow(product_types_model->rowCount());
}

void ProductsForm::on_delete_product_type_but_clicked()
{
    product_types_model->removeRow(ui->product_types_table->currentIndex().row());
}

void ProductsForm::on_change_product_type_icon_but_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Icon"), QDir::currentPath(), tr("Image Files (*.png)"));
    QImage image(fileName,"PNG");
    image = image.scaledToWidth(64,Qt::SmoothTransformation);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    if (!image.save(&buffer, "PNG"))
        return;

    product_types_model->setData(product_types_model->index(ui->product_types_table->currentIndex().row(),product_types_model->fieldIndex("icon")),byteArray,Qt::EditRole);
}

void ProductsForm::on_add_product_but_clicked()
{
    int row = products_model->rowCount();
    products_model->insertRow(row);
    products_model->setData(products_model->index(row,products_model->fieldIndex("product_types_id")),
                            product_types_model->data(product_types_model->index(ui->product_types_table->currentIndex().row(), product_types_model->fieldIndex("id")),Qt::EditRole).toInt());
}

void ProductsForm::on_delete_product_but_clicked()
{
    productsProxyModel->removeRow(ui->products_table->currentIndex().row());
}

void ProductsForm::on_change_product_icon_but_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Icon"), QDir::currentPath(), tr("Image Files (*.png)"));
    QImage image(fileName,"PNG");
    image = image.scaledToWidth(128,Qt::SmoothTransformation);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    if (!image.save(&buffer, "PNG"))
        return;

    productsProxyModel->setData(productsProxyModel->index(ui->products_table->currentIndex().row(),products_model->fieldIndex("icon")),byteArray,Qt::EditRole);
}


void ProductsForm::on_product_suubmit_but_clicked()
{
    if(!products_model->submitAll())
        bDb.debugError(product_types_model->lastError());
}

void ProductsForm::on_product_revert_but_clicked()
{
    products_model->revertAll();
}
