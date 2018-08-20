#ifndef OPTIONSINFORMATION_H
#define OPTIONSINFORMATION_H
#include <QString>


struct ShopNavigateInfo
{
    QString sFilter;
    QString sValue;
    int sSkip = 0;
    int iMsgSize = 0;
    int iCurrentPage = 0;
    int iCount = 8;

};

#endif // OPTIONSINFORMATION_H
