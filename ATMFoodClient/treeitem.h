#ifndef TREEITEM_H
#define TREEITEM_H
#include <QList>
#include <QVariant>

class TreeItem
{
public:
   enum class Type {
       Undefined,
       Food,
       User,
       Date
   };

    explicit TreeItem(const QList<QVariant> &data, TreeItem *parentItem = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    QList<TreeItem*> children()
    {
        return m_childItems;
    }

    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();
    void setType(Type type);
    Type getType() const;
    void replaceData(int iColumn, QVariant vData);

private:
    QList<TreeItem*> m_childItems;
    QList<QVariant> m_itemData;
    TreeItem* m_parentItem;
    Type m_itemType;

};

#endif // TREEITEM_H
