#ifndef USERMODEL_H
#define USERMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QList>
#include <userinformation.h>

class UserModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum EUserColumn
    {
        Name = 0,
        UserName,
        UserRole,
    };

public:
    explicit UserModel(QObject* parent = 0);

    // Overloded
    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;


    // Project
    UserInforMation const& GetUserInfo(int index);

    int GetSize();

    virtual void addItem(UserInforMation const& UserData);
    virtual void removeItem(int index);
    void clear();

    double calculatePrice();

 private:
     QList<UserInforMation> m_UserInformations;
};

#endif // USERMODEL_H
