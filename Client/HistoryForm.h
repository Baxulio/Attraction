#ifndef HISTORYFORM_H
#define HISTORYFORM_H

#include <QWidget>
#include <DatabaseManager.h>

namespace Ui {
class HistoryForm;
}

class SettingsDialog;

class HistoryForm : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryForm(SettingsDialog *bSettings, QWidget *parent = 0);
    ~HistoryForm();

private:
    Ui::HistoryForm *ui;
    DatabaseManager &bDb;
    SettingsDialog *bSettings;
};

#endif // HISTORYFORM_H
