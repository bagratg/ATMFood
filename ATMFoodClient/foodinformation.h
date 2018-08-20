#ifndef FOODINFORMATION_H
#define FOODINFORMATION_H



#include <QString>
#include <QPixmap>

struct FoodInfo
{
    QString foodId;
    QString foodName;
    QString foodType;
    QString foodVendor;
    QString foodPrice;
    QPixmap foodImage;
};

#endif  FOODINFORMATION_H
