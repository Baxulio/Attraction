#include "TransactionItem.h"
#include "ui_TransactionItem.h"

TransactionItem::TransactionItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransactionItem)
{
    ui->setupUi(this);
}

TransactionItem::~TransactionItem()
{
    delete ui;
}
