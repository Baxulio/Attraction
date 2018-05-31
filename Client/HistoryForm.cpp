#include "HistoryForm.h"
#include "ui_HistoryForm.h"

HistoryForm::HistoryForm(SettingsDialog *bSettings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryForm),
    bDb(DatabaseManager::instance()),
    bSettings(bSettings)
{
    ui->setupUi(this);
}

HistoryForm::~HistoryForm()
{
    delete ui;
}
