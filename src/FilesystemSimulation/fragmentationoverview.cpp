#include "fragmentationoverview.h"
#include "BlockWidget.h"  // Include your BlockWidget definition
#include "Toast.h"
#include "mainwindow.h"
#include <QtCore/qforeach.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>

int BLOCK_COUNT = 0;

BlockGridDialog::BlockGridDialog(QWidget *parent)
    : QDialog(parent) {

    // MBR aus dem MainWindow holen
    MainWindow *parentMainWindow = qobject_cast<MainWindow*>(parentWidget());
    master_boot_record = parentMainWindow->getMBR();
    BLOCK_COUNT = parentMainWindow->getBlockCount();
    bool readOnly = parentMainWindow->getReadOnly();

    blockStates =  master_boot_record->get_system()->getBlockStates();

    // Haupt Layout erzeugen
    setWindowTitle("Fragmentierung");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Layout der Matrix setzen
    QWidget *gridContainer = new QWidget(this);
    gridLayout = new QGridLayout(gridContainer);
    gridContainer->setLayout(gridLayout);
    setupBlockGrid();

    // Defragmentierungs Anzeige bauen
    QLabel *label = new QLabel("Fragmentierung:", this);

    QPushButton *defragButton = new QPushButton(this);
    defragButton->setText("Defragmentieren");
    defragButton->setDisabled(readOnly);
    if(readOnly) {
        defragButton->setToolTip("Da auf CDs nicht gelöscht werden kann\nKann auch nicht defragmentiert werden.");
    } else {
        defragButton->setToolTip(nullptr);
    }
    connect(defragButton, &QPushButton::clicked, this, &BlockGridDialog::onDefragButtonClicked);

    QLineEdit *lineEdit = new QLineEdit(this);
    lineEdit->setReadOnly(true);
    lineEdit->setText(QString::number(master_boot_record->get_system()->getFragmentation()) + " %");

    QVBoxLayout *controlLayout = new QVBoxLayout();
    QHBoxLayout *controlLayoutSpinnerTitle = new QHBoxLayout();
    controlLayoutSpinnerTitle->addWidget(label);
    controlLayoutSpinnerTitle->addWidget(lineEdit);
    controlLayout->addLayout(controlLayoutSpinnerTitle);
    controlLayout->addWidget(defragButton);

    // Alles zusammen bauen
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(gridContainer);
    setLayout(mainLayout);
}

void BlockGridDialog::setupBlockGrid() {
    gridLayout->setSpacing(1);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // Zeilen und Reihen berrechnen
    int rows = static_cast<int>(std::sqrt(BLOCK_COUNT));
    int cols = (BLOCK_COUNT + rows - 1) / rows; // Aufrunden

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int index = (row + col) + (row * (rows - 1));

            BlockWidget *block = new BlockWidget();
            block->setBlockId(index);
            block->setStatus(blockStates[index].first, blockStates[index].second);

            connect(block, &BlockWidget::blockClicked, this, &BlockGridDialog::onBlockClicked);

            gridLayout->addWidget(block, row, col);
        }
    }
}

void BlockGridDialog::onBlockClicked(int blockId) {
    BlockState blockState = blockStates[blockId].first;
    BlockState newBlockState;

    bool isOccupied = false;

    switch (blockState) {
    case OCCUPIED:
        isOccupied = true;
        break;
    case FREE:
        newBlockState = RESERVED;
        break;
    case RESERVED:
        newBlockState = DEFECT;
        break;
    case DEFECT:
        newBlockState = FREE;
        break;
    }

    if(isOccupied) {
        Toast *toast = new Toast(this);
        toast->showMessage("Block in Benutzung\nDas Feld kann nicht verändert werden.", 6000);
    } else {
        master_boot_record->get_system()->setBlock(blockId, newBlockState);
        updateWidgetValues();
    }
}

void BlockGridDialog::onDefragButtonClicked() {
    master_boot_record->get_system()->defragDisk();
    updateWidgetValues();
}

void BlockGridDialog::updateWidgetValues() {
    if (!master_boot_record) {
        qDebug() << "Maste boot record not found!";
        return;
    }

    // Update the spinner with the latest fragmentation percentage
    QLineEdit *lineEdit = findChild<QLineEdit*>();
    if (lineEdit) {
        lineEdit->setText(QString::number(master_boot_record->get_system()->getFragmentation()) + " %");
    }

    blockStates = master_boot_record->get_system()->getBlockStates();
    setupBlockGrid();
}
