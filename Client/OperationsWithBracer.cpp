#include "OperationsWithBracer.h"
#include "ui_OperationsWithBracer.h"

OperationsWithBracer::OperationsWithBracer(SettingsDialog *bSettings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OperationsWithBracer),
    bDb(DatabaseManager::instance()),
    bSettings(bSettings)
{
    ui->setupUi(this);
}

OperationsWithBracer::~OperationsWithBracer()
{
    delete ui;
}
