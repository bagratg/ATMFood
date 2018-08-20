#include "ordersmodel.h"
#include <QDebug>

OrdersModel::OrdersModel(QObject *parent):
    QAbstractItemModel(parent)
{

}

int OrdersModel::rowCount(const QModelIndex &parent) const
{

    if(!parent.isValid())
    {
        return m_data.size();
    }

    TreeItem* item = static_cast<TreeItem*>(parent.internalPointer());
    if(item == nullptr) {
        return 0;
    }
    return item->childCount();
/*
    if(item->getType() == TreeItem::Type::User)
    {
        //if(!isWithDate)
       // {
            return item->childCount();
        //}
    }
    else if(item->getType() == TreeItem::Type::Date)
    {
        return m_data[parent.row()]->children().size();
    }
    else if(item->getType() == TreeItem::Type::Food)
    {
        return 0;
    }
    else
    {
        return m_data.size();
    }*/
}


int OrdersModel::columnCount(const QModelIndex &parent) const
{
        return 4;
}


QVariant OrdersModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    int nColumn = index.column();
    TreeItem *pItem = static_cast<TreeItem*>(index.internalPointer());

    switch (role)
    {
        case Qt::DisplayRole:
        {
            return pItem->data(nColumn);
        }

        default:
            return QVariant();
    }
}

Qt::ItemFlags OrdersModel::flags(const QModelIndex &index) const
{

    if(!index.isValid())
    {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex OrdersModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }
    if (!parent.isValid())
    {
        return createIndex(row, column, m_data[row]);
    }

    TreeItem* user = static_cast<TreeItem*>(parent.internalPointer());
    return createIndex(row, column, user->children().at(row));
}


QModelIndex OrdersModel::parent(const QModelIndex &child) const
{

    if(!child.isValid())
    {
        return QModelIndex();
    }

    TreeItem* pChildItem =  static_cast<TreeItem*>(child.internalPointer());
    if(pChildItem == nullptr )
    {
        return QModelIndex();
    }
    TreeItem* pParent = pChildItem->parentItem();

    if (pParent == nullptr)
    {
        return QModelIndex();
    }

    return createIndex(pParent->row(),0,pParent);

}

void OrdersModel::setItems(TreeItem *root, bool isWithDate)
{
    beginResetModel();
    m_data = root->children();

    if(isWithDate)
    {
        for(TreeItem* date : m_data)
        {
            date->setType(TreeItem::Type::Date);
           // qDebug() << date->data(0).toString();
            for(TreeItem* user: date->children())
            {
                user->setType(TreeItem::Type::User);
              //  qDebug() << user->data(0).toString();
                for(TreeItem* food : user->children())
                {
                    food->setType(TreeItem::Type::Food);
                   // qDebug() << food->data(0).toString();
                }
            }
        }
    }
    else
    {
        for(TreeItem* user : m_data)
        {
            user->setType(TreeItem::Type::User);
            for(TreeItem* food : user->children())
            {
                food->setType(TreeItem::Type::Food);
            }
        }
    }
    endResetModel();
}

void OrdersModel::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}


QVariant OrdersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation == Qt::Vertical )
        return QVariant();

    if( role == Qt::DisplayRole )
    {
        switch (section) {
        case 0:
            return tr("Օգտանուն");
        case 1:
            return tr("Վճարման Գումար");
        case 2:
            return tr("գին(դր)");
        case 3:
            return tr("քանակ");
            break;
        default:
            break;
        }
    }

    return QVariant();
}

