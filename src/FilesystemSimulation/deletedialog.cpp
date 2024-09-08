#include "deletedialog.h"
#include "ui_deletedialog.h"

deleteDialog::deleteDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::deleteDialog)
{
    ui->setupUi(this);

    setWindowTitle("Löschen");

    parentMainWindow = qobject_cast<MainWindow*>(parentWidget());
    master_boot_record = parentMainWindow->getMBR();
    selectedTreeItemPath = parentMainWindow->getSelectedTreeItemPath();

    ui->pathToDelete->setText("/" + selectedTreeItemPath);
}

deleteDialog::~deleteDialog()
{
    delete ui;
}

void deleteDialog::on_buttonBox_accepted()
{
    std::string selectedPath = selectedTreeItemPath.toStdString();

    TreeItem* treeItem = parentMainWindow->treeModel->findElement(selectedPath);

    bool success;
    if(treeItem->getIsFolder()) {
        success = master_boot_record->get_system()->removeFolder(selectedPath);
    } else {
        success = master_boot_record->get_system()->removeFile(selectedPath);
    }

    if(success) {
        parentMainWindow->treeModel->removeItem(treeItem);
        parentMainWindow->calculateUsage();
    }
}

