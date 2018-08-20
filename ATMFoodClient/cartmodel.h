#ifndef CARTMODEL_H
#define CARTMODEL_H

#include "foodmodel.h"


class CartModel : public FoodModel
{
    using Base = FoodModel;

    enum EFoodColumn
    {
        FoodCount = FoodPrice + 1
    };

public:
    CartModel(QObject* pParent = nullptr);

public:

    // QAbstractItemModel interface
public:
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    // FoodModel interface
public:
    void addItem(FoodInfo const& FoodData);
    void removeItem(int index) override;

public:
    // geter
    int getCount(int idx);
    double calculatePrice();
private:
    // content
    QList<int>  m_lstFoodsCount;
};

#endif // CARTMODEL_H
