#ifndef ORDERSMODEL_H
#define ORDERSMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <Qlist>
#include <QMap>

#include "order.h"
#include "treeitem.h"

class OrdersModel : public QAbstractItemModel

{
    Q_OBJECT

public:
    enum OrderParentInfo
    {
        username = 0,
        price
    };
public:
    explicit OrdersModel(QObject* parent = 0);

public:
    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

public:
    void setItems(TreeItem *root, bool isWithDate = false);
    // own interface
    void clear();


private:
    QList<TreeItem*> m_data;
//    QMap <QString, QList<Order>> m_Orsders;
//     QList<Order> m_Orders;


    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // ORDERSMODEL_H
