#include "SalesForm.h"
#include "ui_SalesForm.h"
#include <QItemSelectionModel>
#include <QSqlRecord>

#include <QDebug>
#include <QTableView>
#include <QMessageBox>
#include <QSqlDriver>
#include "dialogs/WieagandReaderDialog.h"

SalesForm::SalesForm(SettingsDialog &set, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SalesForm),
    bDb(DatabaseManager::instance()),
    bSetings(set)
{
    ui->setupUi(this);

    qDebug()<<bSetings.activityPointSettings().activityPointNumber;
    typesModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
    productTypesProxyModel.setSourceModel(&typesModel);
    ui->types_listView->setModel(&productTypesProxyModel);

    productsModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
    productsProxyModel.setSourceModel(&productsModel);
    ui->products_listView->setModel(&productsProxyModel);
    ui->products_listView->addAction(ui->add_action);
    ui->products_listView->addAction(ui->remove_action);

    cartProxyModel.setSourceModel(&productsModel);
    ui->cart_table->setModel(&cartProxyModel);
    ui->cart_table->addAction(ui->add_action);
    ui->cart_table->addAction(ui->remove_action);

    connect(&bDb, &DatabaseManager::refresh, this, &SalesForm::on_refresh);

    connect(ui->types_listView->selectionModel(), &QItemSelectionModel::currentRowChanged, [this](QModelIndex cur, QModelIndex prev){
        Q_UNUSED(prev)
        QSqlRecord rec = typesModel.record(cur.row());
        productsProxyModel.setTypeIdFilter(rec.value("id").toInt());
        ui->product_type_label->setText(rec.value("title").toString().toUpper());
    });
    connect(ui->add_action, &QAction::triggered, [this]{
        int column = productsModel.fieldIndex("amount");
        double total_price = ui->total_price_label->text().toDouble();

        if(ui->cart_table->hasFocus()){
            int amount = ui->amount_label->text().toInt();
            if(!amount)return;
            ui->amount_label->setText(QString::number(amount+1));
            int row = ui->cart_table->currentIndex().row();
            cartProxyModel.setData(cartProxyModel.index(row,column),cartProxyModel.data(cartProxyModel.index(row,column), Qt::EditRole).toInt()+1);
            ui->total_price_label->setText(QString::number(total_price+cartProxyModel.data(cartProxyModel.index(row, productsModel.fieldIndex("price")),Qt::EditRole).toDouble()));
            return;
        }

        int row = ui->products_listView->currentIndex().row();
        productsProxyModel.setData(productsProxyModel.index(row, column),productsProxyModel.data(productsProxyModel.index(row, column),Qt::EditRole).toInt()+1);

        ui->total_price_label->setText(QString::number(total_price+productsProxyModel.data(productsProxyModel.index(row, productsModel.fieldIndex("price")),Qt::EditRole).toDouble()));

        int amount = ui->amount_label->text().toInt();
        ui->amount_label->setText(QString::number(amount+1));

        cartProxyModel.invalidate();
        ui->cart_table->reset();


    });

    connect(ui->remove_action, &QAction::triggered, [this]{
        int amount = ui->amount_label->text().toInt();
        if(amount>1)
            ui->amount_label->setText(QString::number(--amount));
        else {
            on_clear_but_clicked();
            return;
        }

        int column = productsModel.fieldIndex("amount");
        double total_price = ui->total_price_label->text().toDouble();

        if(ui->cart_table->hasFocus()){
            int row = ui->cart_table->currentIndex().row();
            int num = cartProxyModel.data(cartProxyModel.index(row, column),Qt::EditRole).toInt();
            if(!num)return;
            cartProxyModel.setData(cartProxyModel.index(row, column),--num);
            ui->total_price_label->setText(QString::number(total_price-cartProxyModel.data(cartProxyModel.index(row, productsModel.fieldIndex("price")),Qt::EditRole).toDouble()));
            return;
        }
        int row = ui->products_listView->currentIndex().row();

        int num = productsProxyModel.data(productsProxyModel.index(row, column),Qt::EditRole).toInt();
        if(!num)return;

        productsProxyModel.setData(productsProxyModel.index(row, column),--num);

        ui->total_price_label->setText(QString::number(total_price-productsProxyModel.data(productsProxyModel.index(row, productsModel.fieldIndex("price")),Qt::EditRole).toDouble()));

        cartProxyModel.invalidate();
        ui->cart_table->reset();
    });

    on_refresh();
    ui->types_listView->setCurrentIndex(productTypesProxyModel.index(0,0));
}

SalesForm::~SalesForm()
{
    delete ui;
}

void SalesForm::on_refresh()
{
    if(!this->isVisible())return;

    on_clear_but_clicked();
    typesModel.setTable("product_types");
    if(!typesModel.select()){
        bDb.debugError(typesModel.lastError());
        return ;
    }
    ui->types_listView->setModelColumn(typesModel.fieldIndex("title"));

    productsModel.setTable("products");
    productsModel.setSort(productsModel.fieldIndex("title"),Qt::AscendingOrder);
    if(!productsModel.select()){
        bDb.debugError(productsModel.lastError());
        return ;
    }
    ui->products_listView->setModelColumn(productsModel.fieldIndex("title"));

    ui->cart_table->hideColumn(productsModel.fieldIndex("id"));
    ui->cart_table->hideColumn(productsModel.fieldIndex("product_types_id"));
    ui->cart_table->hideColumn(productsModel.fieldIndex("icon"));
    ui->cart_table->hideColumn(productsModel.fieldIndex("comment"));
    ui->cart_table->hideColumn(productsModel.fieldIndex("amount"));

    cartProxyModel.setHeaderData(productsModel.fieldIndex("title"),Qt::Horizontal,"Кол-во продукта", Qt::DisplayRole);
    cartProxyModel.setHeaderData(productsModel.fieldIndex("price"),Qt::Horizontal,"Цена", Qt::DisplayRole);
}

void SalesForm::on_back_but_clicked()
{
    emit back();
}

void SalesForm::on_products_listView_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
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
    if(!query.exec(QString("SELECT active_bracers.id, deposit_id, cash "
                           "FROM (active_bracers INNER JOIN deposit ON deposit_id=deposit.id) "
                           "WHERE code=%1 AND NOT ISNULL(enter_time);").arg(code))){
        bDb.debugQuery(query);
        return;
    }
    query.next();

    if(!query.isValid()){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Карта не зарегистрирована или не зафиксирована при входе!"));
        return;
    }

    double total=0;
    QString queryString = "INSERT INTO active_transactions (product_id, quantity, total_price, time, active_bracers_id, activity_point) VALUES";
    for(int i=0; i<n; i++){
        int quantity = cartProxyModel.data(cartProxyModel.index(i, productsModel.fieldIndex("amount")),Qt::EditRole).toInt();
        double total_price = quantity*cartProxyModel.data(cartProxyModel.index(i, productsModel.fieldIndex("price")),Qt::EditRole).toDouble();
        QString product_id = cartProxyModel.data(cartProxyModel.index(i, productsModel.fieldIndex("id")),Qt::EditRole).toString();
        queryString.append(QString(" (%1,%2,%3,SYSDATE(),%4,%5),")
                           .arg(product_id)
                           .arg(quantity)
                           .arg(total_price)
                           .arg(query.value("id").toString())
                           .arg(bSetings.activityPointSettings().activityPointNumber));
        total+=total_price;
    }
    queryString.chop(1);
    queryString.append("; ");

    //    qDebug()<<queryString;
    double cash = query.value("cash").toDouble();
    //comment here to allow negative deposit
    //
    if(total>cash){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Недостаточно средств!"));
        return;
    }
    //

    if(!query.exec(QString("CALL make_payment('%1','%2')")
                   .arg(QString("UPDATE deposit SET cash=%1 WHERE id=%2;")
                        .arg(cash-total)
                        .arg(query.value("deposit_id").toInt()))
                   .arg(queryString))){
        QMessageBox::warning(this, "Неожиданная ситуация",
                             QString("Неудача!"));
        bDb.debugQuery(query);
        return;
    }
    QMessageBox::information(this, "Успех",
                             QString("<font color='green'>Успешно!"));
}

void SalesForm::on_grid_view_but_clicked()
{
    ui->products_listView->setViewMode(QListView::IconMode);
    ui->products_listView->setFlow(QListView::LeftToRight);
}

void SalesForm::on_list_view_but_clicked()
{
    ui->products_listView->setViewMode(QListView::ListMode);
    ui->products_listView->setFlow(QListView::TopToBottom);
}
