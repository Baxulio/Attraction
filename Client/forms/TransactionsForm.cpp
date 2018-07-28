#include "TransactionsForm.h"
#include "ui_TransactionsForm.h"

#include <QDebug>

TransactionsForm::TransactionsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransactionsForm),
    bDb(DatabaseManager::instance()),
    model(new QSqlQueryModel(this)),
    proxyModel(new TransactionsProxyModel(this))
{
    ui->setupUi(this);

    proxyModel->setSourceModel(model);
    ui->tableView->setModel(proxyModel);

    connect(&bDb, &DatabaseManager::refresh, this, &TransactionsForm::on_refresh);

    ui->filter_but->addAction(ui->cashboxAction);

    on_refresh();
}

TransactionsForm::~TransactionsForm()
{
    delete ui;
}

void TransactionsForm::on_refresh()
{
    model->setQuery("SELECT * FROM transactions;");
    if (model->lastError().isValid()){
        bDb.debugError(model->lastError());
        return;
    }

    ui->tableView->hideColumn(0);

    on_clean_but_clicked();
}

void TransactionsForm::on_back_but_clicked()
{
    emit back();
}

void TransactionsForm::on_filter_but_triggered(QAction *arg1)
{
    if(arg1==ui->cashboxAction){
        on_clean_but_clicked();

        on_today_but_clicked();
        proxyModel->setTimeFrom(ui->from_dateTime->dateTime());
        proxyModel->setTimeTo(ui->to_dateTime->dateTime());

        ui->add_ballance_check->setChecked(false);
        proxyModel->setAddBallanceEnabled(false);

        ui->return_debt_check->setChecked(false);
        proxyModel->setReturnDebtEnabled(false);

        ui->return_remainder_check->setChecked(false);
        proxyModel->setReturnRemainderEnabled(false);
        invalidateProxyModel();
        return;
    }
}

void TransactionsForm::on_clean_but_clicked()
{
    clearFields();
    invalidateProxyModel();
}

void TransactionsForm::on_today_but_clicked()
{
    ui->from_dateTime->setDateTime(QDateTime(QDate::currentDate(), QTime(0,0)));
    ui->to_dateTime->setDateTime(QDateTime(QDate::currentDate(), QTime(23,59)));
}

void TransactionsForm::clearFields()
{
    ui->product_lineEdit->clear();
    proxyModel->setProduct("");

    ui->bracer_number_spin->setValue(0);
    proxyModel->setBracerNumebr(0);

    ui->actitivity_point_spin->setValue(0);
    proxyModel->setActivityPoint(0);

    ui->amount_spin->setValue(0);
    proxyModel->setAmount(0);

    ui->from_dateTime->setDateTime(ui->from_dateTime->minimumDateTime());
    proxyModel->setTimeFrom(QDateTime());

    ui->to_dateTime->setDateTime(ui->to_dateTime->maximumDateTime());
    proxyModel->setTimeTo(QDateTime());

    ui->from_money_doubleSpinBox->setValue(0);
    proxyModel->setPriceFrom(0);

    ui->to_money_doubleSpinBox->setValue(0);
    proxyModel->setPriceTo(0);

    ui->money_check->setChecked(true);
    proxyModel->setMoney(true);

    ui->terminal_check->setChecked(true);
    proxyModel->setTerminal(true);

    ui->reg_check->setChecked(true);
    proxyModel->setRegEnabled(true);

    ui->add_ballance_check->setChecked(true);
    proxyModel->setAddBallanceEnabled(true);

    ui->penalty_check->setChecked(true);
    proxyModel->setPenaltyEnabled(true);

    ui->buy_check->setChecked(true);
    proxyModel->setBuyEnabled(true);

    ui->return_debt_check->setChecked(true);
    proxyModel->setReturnDebtEnabled(true);

    ui->return_remainder_check->setChecked(true);
    proxyModel->setReturnRemainderEnabled(true);
}

void TransactionsForm::invalidateProxyModel()
{
    proxyModel->invalidate();

    int col = 0;
    while(model->headerData(col,Qt::Horizontal,Qt::DisplayRole).toString()!="Стоимость")
        col++;

    int rows = proxyModel->rowCount();
    double totalPrice=0;

    for(int i=0; i<rows; i++){
        totalPrice+=proxyModel->data(proxyModel->index(i, col),Qt::EditRole).toDouble();
    }

    ui->info_label->setText(QString("<span style=\" font-size:14pt; color:#55aaff;\">%1 </span>"
                                    "<span style=\" color:#646464;\">Транзакций - </span>"
                                    "<span style=\" font-size:14pt;  color:#55aa00;\">%2 </span>"
                                    "<span style=\" color:#c8c8c8;\">UZS</span>"
                                    "<span style=\" color:#646464;\"> Общая стоимость</span>")
                            .arg(rows)
                            .arg(QString::number(totalPrice,'f',2)));
}

void TransactionsForm::on_filter_but_clicked()
{
    proxyModel->setProduct(ui->product_lineEdit->text());
    proxyModel->setBracerNumebr(ui->bracer_number_spin->value());
    proxyModel->setActivityPoint(ui->actitivity_point_spin->value());
    proxyModel->setAmount(ui->amount_spin->value());

    proxyModel->setTimeFrom(ui->from_dateTime->dateTime());
    proxyModel->setTimeTo(ui->to_dateTime->dateTime());

    proxyModel->setPriceFrom(ui->from_money_doubleSpinBox->value());
    proxyModel->setPriceTo(ui->to_money_doubleSpinBox->value());
    proxyModel->setMoney(ui->money_check->isChecked());
    proxyModel->setTerminal(ui->terminal_check->isChecked());

    proxyModel->setRegEnabled(ui->reg_check->isChecked());
    proxyModel->setAddBallanceEnabled(ui->add_ballance_check->isChecked());
    proxyModel->setPenaltyEnabled(ui->penalty_check->isChecked());
    proxyModel->setBuyEnabled(ui->buy_check->isChecked());
    proxyModel->setReturnDebtEnabled(ui->return_debt_check->isChecked());
    proxyModel->setReturnRemainderEnabled(ui->return_remainder_check->isChecked());

    invalidateProxyModel();
}
