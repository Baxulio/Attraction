#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::apply);
}

void SettingsDialog::apply(){

    updateSettings();
    hide();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setServerSettings(const SettingsDialog::ServerSettings server)
{
    bServerSettings = server;
    ui->server_hostname_line_edit->setText(bServerSettings.host);
    ui->server_password_line_edit->setText(bServerSettings.password);
    ui->server_port_spin_box->setValue(bServerSettings.port);
    ui->server_login_line_edit->setText(bServerSettings.user);
}

void SettingsDialog::setBareerSettings(const SettingsDialog::BareerSettings bareer)
{
    bBareerSettings = bareer;
    ui->bareer_hostname_edit->setText(bBareerSettings.host);
    ui->bareer_port_spin->setValue(bBareerSettings.port);
}

void SettingsDialog::setActivityPointSettings(const SettingsDialog::ActivityPointSettings point)
{
    bActivityPointSettings = point;
    ui->activity_ponit_edit->setValue(bActivityPointSettings.activityPointNumber);
}

void SettingsDialog::updateSettings()
{
    bServerSettings.host = ui->server_hostname_line_edit->text();
    bServerSettings.password = ui->server_password_line_edit->text();
    bServerSettings.port = ui->server_port_spin_box->value();
    bServerSettings.user = ui->server_login_line_edit->text();

    bBareerSettings.host = ui->bareer_hostname_edit->text();
    bBareerSettings.port = ui->bareer_port_spin->value();

    bActivityPointSettings.activityPointNumber = ui->activity_ponit_edit->value();
}
