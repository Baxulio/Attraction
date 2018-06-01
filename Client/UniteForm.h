#ifndef UNITEFORM_H
#define UNITEFORM_H

#include <QWidget>

namespace Ui {
class UniteForm;
}

class UniteForm : public QWidget
{
    Q_OBJECT

public:
    explicit UniteForm(QWidget *parent = 0);
    ~UniteForm();

private:
    Ui::UniteForm *ui;
};

#endif // UNITEFORM_H
