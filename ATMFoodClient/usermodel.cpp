#include "usermodel.h"


UserModel::UserModel(QObject *parent):
    QAbstractTableModel(parent)
{
        m_UserInformations;
}

int UserModel::rowCount(const QModelIndex &parent) const
{
    return m_UserInformations.count();
}

int UserModel::columnCount(const QModelIndex &parent) const
{
   if(parent.isValid())
   {
       return 0 ;
   }
   return 3; // name,type, price, vendor
}

QVariant UserModel::data(const QModelIndex &index, int role) const
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
        case Name:
            return m_UserInformations.at(index.row()).sName;
        case UserName:
            return m_UserInformations.at(index.row()).sUsername;
        case UserRole:
            return m_UserInformations.at(index.row()).sRole;

        default:
            break;
        }

    }
    default:
        return QVariant();
    }

}

void UserModel::addItem(const UserInforMation &UserData)
{
    int row = m_UserInformations.size();
    beginInsertRows( QModelIndex(), row , row );
    m_UserInformations.append(UserData);
    endInsertRows();
}

int UserModel::GetSize()
{
    return m_UserInformations.size();
}

bool UserModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return true; // KAROXA PETQ CHGA DRA HAMAR CHEM GRUM
}

void addItemTable(UserModel fItem);


Qt::ItemFlags UserModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant UserModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation == Qt::Vertical )
        return QVariant();

    if( role == Qt::DisplayRole )
    {
        switch (section) {
        case Name:
            return tr("Անուն");
        case UserName:
            return tr("Ծածկանուն");
        case UserRole:
            return tr("Կատեգորիա");

            break;
        default:
            break;
        }
    }

    return QVariant();
}

const UserInforMation &UserModel::GetUserInfo(int index)
{
    return m_UserInformations[index];
}


void UserModel::removeItem(int index)
{
  //  int row = m_UserInformations.size();
    beginRemoveRows(QModelIndex(), index, index);
    m_UserInformations.takeAt(index);
    endRemoveRows();
}

void UserModel::clear()
{
    beginResetModel();
    m_UserInformations.clear();
    endResetModel();
}

