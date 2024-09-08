#ifndef FRAGMENTATIONOVERVIEW_H
#define FRAGMENTATIONOVERVIEW_H

#include "Backend/MBR/MBR.h"
#include <QDialog>
#include <QGridLayout>

class BlockWidget;  // Forward declaration

class BlockGridDialog : public QDialog {
    Q_OBJECT

public:
    explicit BlockGridDialog(QWidget *parent = nullptr);

private slots:
        void onDefragButtonClicked();

private:
    void setupBlockGrid();
    QGridLayout *gridLayout;
    void onBlockClicked(int blockId);
    MBR *master_boot_record;
    void updateWidgetValues();
    std::map<int, std::pair<BlockState, int>> blockStates;
};

#endif // FRAGMENTATIONOVERVIEW_H
