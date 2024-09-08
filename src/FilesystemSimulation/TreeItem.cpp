#include "TreeItem.h"

// Konstruktor initialisiert ein TreeItem mit den gegebenen Daten und dem parent.
TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent, bool is_folder)
    : m_itemData(data), m_parentItem(parent), is_folder(is_folder) {}

// Destruktor löscht alle children.
TreeItem::~TreeItem() {
    // Alle Kinder Rekursiv löschen.
    qDeleteAll(m_childItems);
    m_childItems.clear();
}

// Fügt ein child zum aktuellen Element hinzu.
void TreeItem::appendChild(TreeItem *child) {
    m_childItems.append(child);
}

// Gibt das child an der angegebenen row zurück.
TreeItem *TreeItem::child(int row) {
    return m_childItems.value(row);
}

// Gibt das child an der angegebenen row zurück.
bool TreeItem::getIsFolder() {
    return is_folder;
}

// Gibt die Anzahl der child zurück.
int TreeItem::childCount() const {
    return m_childItems.count();
}

// Gibt die Anzahl der columns an Daten zurück, die dieses Element hält.
int TreeItem::columnCount() const {
    return m_itemData.count();
}

// Gibt die Daten für die angegebene column zurück.
QVariant TreeItem::data(int column) const {
    return m_itemData.value(column);
}

// Gibt die Zeilennummer dieses Elements in der Liste der Children seines parents zurück.
int TreeItem::row() const {
    // Es handelt sich um die Root.
        if (!m_parentItem) {
            return -1;
        }
        int index = m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));
        if (index != -1) {
            return index;
        } else {
            qWarning() << "TreeItem::row() - Item not found in parent's child list!";
            return -1;
        }
}

// Gibt das parent zurück.
TreeItem *TreeItem::parentItem() {
    return m_parentItem;
}

// Löscht ein spezifisches Kind aus dem Item
bool TreeItem::removeChild(TreeItem *child) {
    int index = m_childItems.indexOf(child);
    if (index != -1) {
        m_childItems.removeAt(index);
        delete child;
        return true;
    }
    return false;
}

// Removes all children from the TreeItem
void TreeItem::clearChildren() {
    qDeleteAll(m_childItems);
    m_childItems.clear();
}

// Sortiert die Kinder eines Elements Alphabetisch.
void TreeItem::sortChildren() {
    std::sort(m_childItems.begin(), m_childItems.end(), [](TreeItem *a, TreeItem *b) {
        return a->data(0).toString() < b->data(0).toString();
    });
}
