#include "CashForm.h"
#include "ui_CashForm.h"

CashForm::CashForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CashForm)
{
    ui->setupUi(this);
}

CashForm::~CashForm()
{
    delete ui;
}
