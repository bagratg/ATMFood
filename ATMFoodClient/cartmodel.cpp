#include "cartmodel.h"

CartModel::CartModel(QObject *pParent)
    : Base( pParent )
{
}

int CartModel::columnCount(const QModelIndex &parent) const
{
    return Base::columnCount(parent) + 1;
}

QVariant CartModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    int nColumn = index.column();

    if(nColumn == FoodCount && role == Qt::DisplayRole )
        return m_lstFoodsCount.at(index.row());

    return Base::data(index, role);
}

void CartModel::addItem(const FoodInfo &FoodData)
{
    QString sFoodId = FoodData.foodId;
    Q_ASSERT( !sFoodId.isEmpty() );

    // check already exists
    int nFoodIndex = -1;
    for( int i = 0; i < Base::GetSize(); ++i )
    {
        if( Base::GetFoodInfo( i ).foodId == FoodData.foodId )
        {
            nFoodIndex = i;
            break;
        }
    }

    if( nFoodIndex >= 0)
    {
        // item exists
        Q_ASSERT(nFoodIndex < m_lstFoodsCount.size());
        m_lstFoodsCount[nFoodIndex]++;
        // notify about data change
        emit dataChanged( index(nFoodIndex, FoodCount), index(nFoodIndex, FoodCount) );
    }
    else
    {
        m_lstFoodsCount.append(1);
        Base::addItem( FoodData );
    }
}

void CartModel::removeItem(int index)
{
    Q_ASSERT( index >= 0 && index < m_lstFoodsCount.size() );
    if( index < 0 || index >= m_lstFoodsCount.size() )
        return;

    // remove count
    m_lstFoodsCount.removeAt( index );
    Base::removeItem( index );
}

int CartModel::getCount(int idx)
{
    return m_lstFoodsCount[idx];
}

double CartModel::calculatePrice()
{
    double tmp;
    for(int i = 0; i < m_lstFoodsCount.size();i++)
    {
        tmp += GetFoodInfo(i).foodPrice.toInt() * m_lstFoodsCount[i] ;
    }
    return tmp;
}

QVariant CartModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation == Qt::Vertical )
        return QVariant();
    if(section == FoodCount && role == Qt::DisplayRole )
        return tr("Քանակ");

    return Base::headerData(section, orientation,role);
}
