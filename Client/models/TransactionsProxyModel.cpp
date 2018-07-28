#include "TransactionsProxyModel.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>

TransactionsProxyModel::TransactionsProxyModel(QObject *parent):
    QSortFilterProxyModel(parent),
    bracer_number(0),
    amount(0),
    activity_point(0),
    product(""),
    price_from(0),
    price_to(0),
    terminal(false),
    money(false),
    reg(false),
    buy(false),
    penalty(false),
    addBallance(false),
    returnDebt(false),
    returnRemainder(false)
{

}

void TransactionsProxyModel::invalidate()
{
    invalidateFilter();
}

bool TransactionsProxyModel::dateInRange(const QDateTime &dateTime) const
{
    return (!time_from.isValid() || dateTime>=time_from)
            && (!time_to.isValid() || dateTime<=time_to);
}

bool TransactionsProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent)
    QSqlQueryModel *model = static_cast<QSqlQueryModel*>(sourceModel());

    if(bracer_number && model->record(source_row).value("Номер браслета").toInt()!=bracer_number)
        return false;
    if(amount && model->record(source_row).value("Количество").toInt()!=amount)
        return false;
    if(activity_point && model->record(source_row).value("Пункт").toInt()!=activity_point)
        return false;
    if(product!="" && !model->record(source_row).value("Продукт").toString().startsWith(product,Qt::CaseInsensitive))
        return false;

    double price = model->record(source_row).value("Стоимость").toDouble();
    if(price_to && (price_from>price || price_to<price))
        return false;

    QString term = model->record(source_row).value("Метод оплаты").toString();

    if(!terminal && term=="Терминал")
        return false;
    if(!money && term=="Наличные")
        return false;

    QString type = model->record(source_row).value("Тип").toString();

    if(!reg && type=="Регистрация")
        return false;
    if(!buy && type=="Покупка")
        return false;
    if(!penalty && type=="Штраф")
        return false;
    if(!addBallance && type=="Пополнение баланса")
        return false;
    if(!returnDebt && type=="Оплата")
        return false;
    if(!returnRemainder && type=="Возврат остатка")
        return false;

    if(!dateInRange(model->record(source_row).value("Время").toDateTime()))
        return false;

    return true;
}
