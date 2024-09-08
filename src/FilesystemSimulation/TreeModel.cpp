#include <iostream>
#include <ostream>

#include "TreeModel.h"
#include "TreeItem.h"

#include "helpers/PathHelper.h"

// Konstruktor erstellt das Modell und initialisiert es mit der root.
TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent) {
    QList<QVariant> rootData;
    rootData << "Dateien";
    rootItem = new TreeItem(rootData, nullptr, true);
}

// Destructor löscht das root und alle seine Kind-Elemente.
TreeModel::~TreeModel() {
    delete rootItem;
}

TreeItem* TreeModel::getRootItem() const {
    if (!rootItem) {
        std::cout << "rootItem not available" << std::endl;
        qWarning("Root item is not initialized!");
        return nullptr;
    }
    return rootItem;
}


// Gibt die Anzahl der Spalten für das angegebene Parent zurück.
int TreeModel::columnCount(const QModelIndex &parent) const {
    if (parent.column() > 0)
        return 0;
    if (!parent.isValid())
        return rootItem->columnCount();
    return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
}

// Gibt die Daten für die angegebene Index-Position zurück.
QVariant TreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    // FIXME Ich bin mir noch unsicher ob wir das wirklich brauchen. Benötigt weiterer Forschung.
    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->data(index.column());
}

// Gibt die Flags für den gegebenen Index zurück.
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

// Gibt die Header-Daten für die angegebene Spalte zurück.
QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

// Gibt den Index für die angegebene Zeile und Spalte zurück.
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

// Gibt den Index des parents für das angegebene childs zurück.
QModelIndex TreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    if (!childItem || !isValidTreeItem(childItem))
        return QModelIndex();

    TreeItem *parentItem = childItem->parentItem();
    if (parentItem == rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

// Gibt die Anzahl der Zeilen für das angegebene Pranet zurück.
int TreeModel::rowCount(const QModelIndex &parent) const {
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

bool TreeModel::isValidTreeItem(TreeItem *item) const {
    if (!item) {
        return false;
    }

    if (item == rootItem) {
        return true; // Die Root ist valide.
    }

    TreeItem *parent = item->parentItem();
    return parent != nullptr;
}

// Eine Element anhand eines VectorPath finden.
TreeItem* TreeModel::findElement(const std::vector<std::string> &path) const {
    TreeItem *currentItem = rootItem;

    for (const std::string &key : path) {
        bool found = false;
        for (int i = 0; i < currentItem->childCount(); ++i) {
            TreeItem *child = currentItem->child(i);
            if (child->data(0) == QString::fromStdString(key)) {
                currentItem = child;
                found = true;
                break;
            }
        }
        if (!found) {
            return nullptr;  // Element not found
        }
    }

    return currentItem;
}

TreeItem* TreeModel::findElement(std::string &path_string) const {
    std::vector<std::string> path = PathHelper::split(path_string, '/');
    return findElement(path);
}

void TreeModel::addStringPathToTree(std::string &path, bool IS_FOLDER) {
    std::string prefix = "/root/";
    if (path.find(prefix) == 0) {
        path.erase(0, prefix.length());
    }

    if(path.empty()) {
        return;
    }

    std::vector<std::string> files = PathHelper::split(path, '/');
    prefix.pop_back();

    TreeItem* rootItem = getRootItem();
    std::vector<std::string> currentPath;
    bool isFolder = true;

    for (int i = 0; i < files.size(); i++) {
        currentPath.push_back(files[i]);

        if((i == files.size() - 1) && !IS_FOLDER) {
            isFolder = false;
        }

        if(!findElement(currentPath)) {
            QList<QVariant> data;
            data << QString::fromStdString(currentPath.back());
            TreeItem* newTreeItem = new TreeItem(data, rootItem, isFolder);

            QModelIndex insertIndex = createIndex(rootItem->row(), 0, rootItem);
            int position = rootItem->childCount();

            beginInsertRows(insertIndex, position, position);
            rootItem->appendChild(newTreeItem);
            endInsertRows();

            rootItem->sortChildren();

            rootItem = newTreeItem;
        } else {
            rootItem = findElement(currentPath);
        }
    }

    emit layoutChanged();
}

void TreeModel::clearTree() {

    rootItem->clearChildren();

}

void TreeModel::removeItem(TreeItem* itemToRemove) {
    if (!itemToRemove || itemToRemove == rootItem) {
        qWarning("Cannot remove the root item or null item.");
        return;
    }

    TreeItem* parentItem = itemToRemove->parentItem();
    if (!parentItem) {
        qWarning("Item to remove has no parent.");
        return;
    }

    parentItem->removeChild(itemToRemove);

    emit layoutChanged();
}

// initialisierung beispiel TreeModel.
void TreeModel::setupModelData(TreeItem *parent) {
    QList<QVariant> data;
}
