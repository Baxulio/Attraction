#include "Monitoring.h"
#include "ui_Monitoring.h"

Monitoring::Monitoring(SettingsDialog *bSettings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Monitoring),
    bDb(DatabaseManager::instance()),
    bSettings(bSettings)
{
    ui->setupUi(this);
}

Monitoring::~Monitoring()
{
    delete ui;
}
