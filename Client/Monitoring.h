#ifndef MONITORING_H
#define MONITORING_H

#include <QWidget>
#include <DatabaseManager.h>

namespace Ui {
class Monitoring;
}

class SettingsDialog;

class Monitoring : public QWidget
{
    Q_OBJECT

public:
    explicit Monitoring(SettingsDialog *bSettings, QWidget *parent = 0);
    ~Monitoring();

private:
    Ui::Monitoring *ui;
    DatabaseManager &bDb;
    SettingsDialog *bSettings;
};

#endif // MONITORING_H
