#include "UniteForm.h"
#include "ui_UniteForm.h"

UniteForm::UniteForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UniteForm)
{
    ui->setupUi(this);
}

UniteForm::~UniteForm()
{
    delete ui;
}
