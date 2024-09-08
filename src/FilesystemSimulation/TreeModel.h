#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

class TreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit TreeModel(QObject *parent = nullptr);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    TreeItem* getRootItem() const;
    TreeItem* findElement(const std::vector<std::string> &path) const;
    TreeItem* findElement(std::string &path_string) const;
    void extracted();
    void addStringPathToTree(std::string &path, bool IS_FOLDER);
    void setupModelData(TreeItem *parent);
    void removeItem(TreeItem* itemToRemove);
    void clearTree();

private:
    TreeItem *rootItem;
    bool isValidTreeItem(TreeItem *item) const;
};

#endif // TREEMODEL_H
