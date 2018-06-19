#include "SalesForm.h"
#include "ui_SalesForm.h"
#include <QItemSelectionModel>
#include <QSqlRecord>

#include <QDebug>
#include <QTableView>
#include <QMessageBox>
#include <QSqlDriver>
#include "dialogs/WieagandReaderDialog.h"

SalesForm::SalesForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SalesForm),
    bDb(DatabaseManager::instance())
{
    ui->setupUi(this);

    typesModel.setEditStrategy(SalesModel::OnManualSubmit);
    ui->types_listView->setModel(&typesModel);

    productsModel.setEditStrategy(SalesModel::OnManualSubmit);
    productsProxyModel.setSourceModel(&productsModel);

    ui->products_listView->setModel(&productsProxyModel);
    ui->products_listView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->products_listView->addAction(ui->add_action);
    ui->products_listView->addAction(ui->remove_action);

    cartProxyModel.setSourceModel(&productsModel);
    ui->cart_table->setModel(&cartProxyModel);

    connect(&bDb, &DatabaseManager::refresh, this, &SalesForm::on_refresh);
    connect(ui->types_listView->selectionModel(), &QItemSelectionModel::currentRowChanged, [this](QModelIndex cur, QModelIndex prev){
        Q_UNUSED(prev)
        QSqlRecord rec = typesModel.record(cur.row());
        productsProxyModel.setFilterFixedString(rec.value("id").toString());
        ui->product_type_label->setText(rec.value("title").toString().toUpper());
    });
    connect(ui->add_action, &QAction::triggered, [this]{
        int row = ui->products_listView->currentIndex().row();
        int column = productsModel.fieldIndex("amount");

        productsModel.setData(productsModel.index(row, column),productsModel.data(productsModel.index(row, column),Qt::EditRole).toInt()+1);
        int amount = ui->amount_label->text().toInt();
        double total_price = ui->total_price_label->text().toDouble();
        ui->amount_label->setText(QString::number(amount+1));
        ui->total_price_label->setText(QString::number(total_price+productsModel.data(productsModel.index(row, productsModel.fieldIndex("price")),Qt::EditRole).toDouble()));

        cartProxyModel.invalidate();
    });

    connect(ui->remove_action, &QAction::triggered, [this]{
        int row = ui->products_listView->currentIndex().row();
        int column = productsModel.fieldIndex("amount");

        int num = productsModel.data(productsModel.index(row, column),Qt::EditRole).toInt();
        if(!num)return;

        productsModel.setData(productsModel.index(row, column),--num);

        int amount = ui->amount_label->text().toInt();
        double total_price = ui->total_price_label->text().toDouble();
        if(amount>1){
            ui->amount_label->setText(QString::number(--amount));
            ui->total_price_label->setText(QString::number(total_price-productsModel.data(productsModel.index(row, productsModel.fieldIndex("price")),Qt::EditRole).toDouble()));
        }
        else {
            on_clear_but_clicked();
        }
        cartProxyModel.invalidate();
    });
    on_refresh();
}

SalesForm::~SalesForm()
{
    delete ui;
}

void SalesForm::on_refresh()
{
    on_clear_but_clicked();
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
    ui->add_action->trigger();
}

void SalesForm::on_lineEdit_returnPressed()
{
    int rows = productsProxyModel.rowCount();
    for(int i=0; i<rows; i++)
        if(productsProxyModel.index(i,ui->products_listView->modelColumn()).data().toString().startsWith(ui->lineEdit->text(),Qt::CaseInsensitive)){
            ui->products_listView->setCurrentIndex(productsProxyModel.index(i,ui->products_listView->modelColumn()));
            return;
        }
}

void SalesForm::on_clear_but_clicked()
{
    productsModel.revertAll();
    ui->amount_label->setText("0");
    ui->total_price_label->setText("0");
}

void SalesForm::on_search_but_clicked()
{
    on_lineEdit_returnPressed();
}

void SalesForm::on_make_order_but_clicked()
{
    int n=cartProxyModel.rowCount();
    if(n==0){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Корзина пуста!"));
        return;
    }

    WieagandReaderDialog dialog(this);
    if(dialog.exec() != QDialog::Accepted){
        return;
    }
    quint32 code=dialog.getCode();
    if(code==0){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Неверный ввод!"));
        return;
    }

    QSqlQuery query;
    if(!query.exec(QString("SELECT active_bracers.id, deposit_id, cash FROM (active_bracers INNER JOIN deposit ON deposit_id=deposit.id) WHERE code=%1;").arg(code))){
        bDb.debugQuery(query);
        return;
    }
    query.next();

    if(!query.isValid()){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Карта не зарегистрирована!"));
        return;
    }

    double total=0;
    QString queryString = "INSERT INTO active_transactions (product_id, total_price, time, active_bracers_id) VALUES";
    for(int i=0; i<n; i++){
        double total_price = cartProxyModel.data(cartProxyModel.index(i, productsModel.fieldIndex("amount"))).toInt()
                *cartProxyModel.data(cartProxyModel.index(i, productsModel.fieldIndex("price"))).toDouble();
        QString product_id = cartProxyModel.data(cartProxyModel.index(i, productsModel.fieldIndex("id"))).toString();
        queryString.append(QString(" (%1,%2,SYSDATE(),%3),").arg(product_id).arg(total_price).arg(query.value("id").toString()));
        total+=total_price;
    }
    queryString.chop(1);
    queryString.append("; ");

    double cash = query.value("cash").toDouble();
    if(total>cash){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Недостаточно средств!"));
        return;
    }

    if(!query.exec(QString("CALL make_payment(%1,%2)").arg(QString("UPDATE deposit SET cash=%1 WHERE id=%2;").arg(cash-total).arg(query.value("deposit_id").toInt()))
                   .arg(queryString))){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Неудача!"));
        bDb.debugQuery(query);
        return;
    }
}
