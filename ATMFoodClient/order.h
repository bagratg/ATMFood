#ifndef ORDER_H
#define ORDER_H

#include "foodmodel.h"
#include <QString>

struct Order
{
    QString sUserName;
    FoodModel* pFoodModel;
};
struct SinglOrder
{
    FoodInfo fInfo;
    int count;
};


#endif // ORDER_H
