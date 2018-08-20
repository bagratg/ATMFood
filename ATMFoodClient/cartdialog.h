#ifndef CARTDIALOG_H
#define CARTDIALOG_H

#include <QDialog>
#include <QList>
#include <cartmodel.h>
#include <foodinformation.h>
#include "order.h"

namespace Ui {
class CartDialog;
}

class CartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CartDialog(QWidget *parent = 0);
    ~CartDialog();

public:
    void generateDialog(QList<SinglOrder> &mapOrders);

private:
    CartModel* m_cartModel;


private:
    Ui::CartDialog *ui;
};

#endif // CARTDIALOG_H
