#ifndef OPERATIONSWITHBRACER_H
#define OPERATIONSWITHBRACER_H

#include <QWidget>
#include <DatabaseManager.h>

namespace Ui {
class OperationsWithBracer;
}

class SettingsDialog;

class OperationsWithBracer : public QWidget
{
    Q_OBJECT

public:
    explicit OperationsWithBracer(SettingsDialog *bSettings, QWidget *parent = 0);
    ~OperationsWithBracer();

private:
    Ui::OperationsWithBracer *ui;
    DatabaseManager &bDb;
    SettingsDialog *bSettings;
};

#endif // OPERATIONSWITHBRACER_H
