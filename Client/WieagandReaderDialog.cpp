#include "WieagandReaderDialog.h"
#include "ui_WieagandReaderDialog.h"
#include <QDebug>
WieagandReaderDialog::WieagandReaderDialog(QWidget *parent, const QString &title) :
    QDialog(parent),
    ui(new Ui::WieagandReaderDialog)
{
    ui->setupUi(this);
    ui->label->setText("<p align=\"center\"><span style=\" font-size:12pt;\">"+title+"</span></p>");
}

WieagandReaderDialog::~WieagandReaderDialog()
{
    delete ui;
}

quint32 WieagandReaderDialog::getCode()
{
    return ui->lineEdit->text().section(",",-1,-1,QString::SectionSkipEmpty).remove('?').toULong();
}
