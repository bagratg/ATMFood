#ifndef FOODVIEW_H
#define FOODVIEW_H

#include <QAbstractTableModel>
#include <QObject>
#include <QList>
#include <foodinformation.h>

class FoodModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum EFoodColumn
    {
        FoodName = 1,
        FoodType,
        FoodVendor,
        FoodPrice
    };

public:
    explicit FoodModel(QObject* parent = 0);

    // Overloded
    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;


    // Project
    FoodInfo const& GetFoodInfo(int index);

    int GetSize();

    virtual void addItem(FoodInfo const& FoodData);
    virtual void removeItem(int index);
    void clear();

    double calculatePrice();

 private:
     QList<FoodInfo> m_FoodInformations;
};

#endif // FOODVIEW_H
