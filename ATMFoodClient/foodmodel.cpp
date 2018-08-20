#include "FoodModel.h"

#include <qlabel.h>



FoodModel::FoodModel(QObject *parent):
    QAbstractTableModel(parent)
{
        m_FoodInformations;
}

int FoodModel::rowCount(const QModelIndex &parent) const
{
    return m_FoodInformations.count();
}

int FoodModel::columnCount(const QModelIndex &parent) const
{
   if(parent.isValid())
   {
       return 0 ;
   }
   return 5; // name,type, price, vendor
}

QVariant FoodModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    int nColumn = index.column();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        switch (nColumn) {
        case FoodName:
            return m_FoodInformations.at(index.row()).foodName;
        case FoodType:
            return m_FoodInformations.at(index.row()).foodType;
        case FoodVendor:
            return m_FoodInformations.at(index.row()).foodVendor;
        case FoodPrice:
            return m_FoodInformations.at(index.row()).foodPrice;
//        case FoodId:
//            return m_FoodInformations.at(index.row()).foodId;
        default:
            break;
        }

    }
    case Qt::DecorationRole:
    {
        if(nColumn == 0)
        {
            return m_FoodInformations.at(index.row()).foodImage;
        }
    }
    default:
        return QVariant();
    }

}

void FoodModel::addItem(const FoodInfo &FoodData)
{
    int row = m_FoodInformations.size();
    beginInsertRows( QModelIndex(), row , row );
    m_FoodInformations.append(FoodData);
    endInsertRows();
}

int FoodModel::GetSize()
{
    return m_FoodInformations.size();
}

bool FoodModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return true; // KAROXA PETQ CHGA DRA HAMAR CHEM GRUM
}

void addItemTable(FoodModel fItem);


Qt::ItemFlags FoodModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant FoodModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation == Qt::Vertical )
        return QVariant();

    if( role == Qt::DisplayRole )
    {
        switch (section) {
        case FoodName:
            return tr("Անվանում");
        case FoodType:
            return tr("Տիպ");
        case FoodVendor:
            return tr("Մատակարար");
        case FoodPrice:
            return tr("Գին / (դր)");
            break;
        default:
            break;
        }
    }

    return QVariant();
}

const FoodInfo &FoodModel::GetFoodInfo(int index)
{
    return m_FoodInformations[index];
}


void FoodModel::removeItem(int index)
{
  //  int row = m_FoodInformations.size();
    beginRemoveRows(QModelIndex(), index, index);
    m_FoodInformations.takeAt(index);
    endRemoveRows();
}

void FoodModel::clear()
{
    beginResetModel();
    m_FoodInformations.clear();
    endResetModel();
}

double FoodModel::calculatePrice()
{
   double dAllPrice = 0;
   for(int i = 0; i < m_FoodInformations.count();i++)
   {
         dAllPrice +=m_FoodInformations[i].foodPrice.toInt();
   }
   return dAllPrice;
}

