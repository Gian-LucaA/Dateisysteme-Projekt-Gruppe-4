#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>

class TreeItem {
public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem *parent, bool is_folder);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();
    void sortChildren();
    void removeChild(int row);
    bool getIsFolder();
    bool removeChild(TreeItem *child);
    void clearChildren();

private:
    QList<TreeItem*> m_childItems;
    QList<QVariant> m_itemData;
    TreeItem *m_parentItem;
    bool is_folder;
};

#endif // TREEITEM_H
