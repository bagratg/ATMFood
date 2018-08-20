#include "cartdialog.h"
#include "ui_cartdialog.h"

#include <QFile>
#include <QFileDialog>

CartDialog::CartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CartDialog)
{
    ui->setupUi(this);
    m_cartModel = new CartModel(this);
    ui->ViewAllOrders->setModel(m_cartModel);
}

CartDialog::~CartDialog()
{
    delete ui;
}

void CartDialog::generateDialog(QList<SinglOrder> & m_AllOrderedProducts)
{

    for(int i = 0; i < m_AllOrderedProducts.size(); i++)
    {
        for(int j = 0; j < m_AllOrderedProducts[i].count;j++)
        {
            m_cartModel->addItem(m_AllOrderedProducts[i].fInfo);
        }
    }
    QString sPrice = QString::number(m_cartModel->calculatePrice());
    ui->PriceLabel->setText(sPrice);
}

