#include <QTreeView>
#include <string>
#include <cstdlib>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Backend/MBR/MBR.h"
#include "copydialog.h"
#include "fragmentationoverview.h"
#include "addfolder.h"
#include "deletedialog.h"
#include "helpers/DateTimeHelper.h"
#include "helpers/PathHelper.h"
#include "addfile.h"

using namespace std;

int BLOCK_SIZE = 512;
int DISK_SIZE = 64 * BLOCK_SIZE;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , stackedWidget(new QStackedWidget(this))
    , treeModel(new TreeModel())
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->treeView->setModel(treeModel);

    delete stackedWidget;
    stackedWidget = ui->stackedWidget;

    selectedTreeItemPath = nullptr;
    master_boot_record = new MBR();

    master_boot_record->set_system(0); // FAT
    selctedSystem = 0;
    master_boot_record->get_system()->createSystem(DISK_SIZE, BLOCK_SIZE);
    InitializeTree(master_boot_record);

    master_boot_record->set_system(1); // INode
    selctedSystem = 1;
    master_boot_record->get_system()->createSystem(DISK_SIZE, BLOCK_SIZE);
    InitializeTree(master_boot_record);

    master_boot_record->set_system(2); // CD-Rom
    selctedSystem = 2;
    master_boot_record->get_system()->createSystem(DISK_SIZE, BLOCK_SIZE);
    InitializeTree(master_boot_record);

    if (!stackedWidget) {
        qCritical() << "QStackedWidget not found!";
        return;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete stackedWidget;
    delete treeModel;
    delete master_boot_record;
}

MBR* MainWindow::getMBR() {
    return master_boot_record;
}

QString MainWindow::getSelectedTreeItemPath() {
    return selectedTreeItemPath;
}

bool MainWindow::getReadOnly() {
    return readOnly;
}

int MainWindow::getSystem() {
    return selctedSystem;
}

void MainWindow::on_addItems_clicked()
{
    AddTreeItem dialog(this);
    dialog.exec();
}


void MainWindow::on_pushButton_clicked()
{
    addFolder dialog(this);
    dialog.exec();
}

void MainWindow::on_deleteButton_clicked()
{
    if(selectedTreeItemPath != nullptr) {
        deleteDialog dialog(this);
        dialog.exec();
    } else {
        qWarning() << "Bitte wählen Sie zuerst\neine zu löschende Datei oder einen Ordner!";
    }
}

void MainWindow::on_copyButton_clicked()
{
    std::string selectedTreeItemPathString = selectedTreeItemPath.toStdString();

    if(selectedTreeItemPathString == "") {
        qWarning() << "Bitte wählen Sie zuerst eine zu kopierende Datei!";
        return;
    }

    TreeItem* selectedTreeItem = treeModel->findElement(selectedTreeItemPathString);

    if(selectedTreeItem == nullptr) {
        qWarning() << "Bitte wählen Sie zuerst eine zu kopierende Datei!";
        return;
    }

    if(selectedTreeItem->getIsFolder()) {
        qWarning() << "Bitte wählen Sie eine Datei!\nDas Kopieren von Ordnern ist nicht möglich.";
        return;
    }

    CopyDialog dialog(this);
    dialog.exec();
}



void MainWindow::on_inodeButton_clicked()
{
    master_boot_record->set_system(1); // System INode wählen
    selctedSystem = 1;
    readOnly = false;
    ui->stackedWidget->setCurrentIndex(1);

    calculateUsage();

    PopulateTree();
}

void MainWindow::on_fatButton_clicked()
{
    master_boot_record->set_system(0); // System FAT wählen
    selctedSystem = 0;
    readOnly = false;
    ui->stackedWidget->setCurrentIndex(1);

    calculateUsage();

    PopulateTree();
}

void MainWindow::on_fatButton_CD_clicked()
{
    master_boot_record->set_system(2); // System der CD-Rom wählen
    selctedSystem = 2;
    readOnly = true;
    ui->stackedWidget->setCurrentIndex(1);

    calculateUsage();

    PopulateTree();
}

void MainWindow::InitializeTree(MBR* master_boot_record) {
    std::string initDateTimeMS = std::to_string(DateTimeHelper::getCurrentDateTimeMS());

    switch (selctedSystem) {
    case 0: // FAT
        master_boot_record->get_system()->createFile(501, "home/Documents/Apfel.txt", "txt", "text", "r", "root");
        master_boot_record->get_system()->createFile(501, "home/Documents/Birne.txt", "txt", "text", "r", "root");
        master_boot_record->get_system()->createFile(501, "home/Documents/Kartofel.png", "png", "image", "r", "root");
        break;
    case 1: // INode
        master_boot_record->get_system()->createFile(1024, "home/Games/cheats.txt", "txt", "text", "r", "root");
        master_boot_record->get_system()->createFile(1024, "home/Music/Wasser.mp3", "mp3", "sound", "r", "root");
        break;
    case 2: // CD
        master_boot_record->get_system()->createFile(1024, "Photos/Family/Muttter.png", "png", "image", "r", "root");
        master_boot_record->get_system()->createFile(1024, "Photos/Family/Vater.png", "png", "image", "r", "root");
        master_boot_record->get_system()->createFile(2048, "Photos/Family/Oma.png", "png", "image", "r", "root");
        master_boot_record->get_system()->createFile(501, "Photos/Tiere/Hund.png", "png", "image", "r", "root");
        master_boot_record->get_system()->createFile(501, "Photos/Tiere/Katze.png", "png", "image", "r", "root");
        break;
    }

}

void MainWindow::PopulateTree() {
    std::vector<std::string> directory = master_boot_record->get_system()->getDirectoryTree();

    foreach (std::string path, directory) {
        bool isFolder = path.back() == '/';
        treeModel->addStringPathToTree(path, isFolder);
    }
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{

    selectedTreeItemPath = PathHelper::buildPath(index);

    std::string findablePath = selectedTreeItemPath.toStdString();
    TreeItem* treeItem = treeModel->findElement(findablePath);

    if(!treeItem->getIsFolder()) {
        FileInformationInterface* file = master_boot_record->get_system()->getFile(selectedTreeItemPath.toStdString());

        ui->fileCreation->show();
        ui->fileChanged->show();
        ui->label_dates->show();

        ui->fileType->setText(QString::fromStdString(file->get_file_kind()));
        ui->fileName->setText(QString::fromStdString(file->get_file_name()));
        ui->fileCreation->setText(QString::fromStdString(DateTimeHelper::msToTimestamp(file->get_file_creation_time())));
        ui->fileChanged->setText(QString::fromStdString(DateTimeHelper::msToTimestamp(file->get_file_modification_time())));
        ui->fileSize->setText(QString::number(file->get_file_size()) + " Byte");
        ui->fileSIze_blocks->setText(QString::number((file->get_file_size() + BLOCK_SIZE -1)/ BLOCK_SIZE) + " Blöcke");
    } else {
        ui->fileType->setText("folder");

        FolderInformationInterface* folder = master_boot_record->get_system()->getFolder(selectedTreeItemPath.toStdString());

        ui->fileName->setText(QString::fromStdString(folder->get_folder_name()));
        ui->fileSize->setText(QString::number(folder->get_folder_size()) + " Byte");
        ui->fileSIze_blocks->setText(QString::number((folder->get_folder_size() + BLOCK_SIZE -1)/ BLOCK_SIZE) + " Blöcke");

        ui->fileCreation->hide();
        ui->fileChanged->hide();
        ui->label_dates->hide();

        ui->fileCreation->setText("");
        ui->fileChanged->setText("");
    }
}

void MainWindow::on_showFragmentation_clicked()
{
    BlockGridDialog *dialog = new BlockGridDialog(this);
    dialog->exec();
    delete dialog;
}


void MainWindow::calculateUsage() {
    size_t usedSpace = master_boot_record->get_system()->getPartitionUsage();
    ui->progressBar->setValue(usedSpace * 100 / DISK_SIZE);
    ui->usageString->setText(QString::fromStdString(std::to_string(usedSpace * 100 / DISK_SIZE) + " %"));
}

int MainWindow::getBlockCount() {

    int blockCount = DISK_SIZE/BLOCK_SIZE;
    return blockCount;
}

void MainWindow::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    treeModel->clearTree();
}


void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    if(arg1 == 1) {
        ui->deleteButton->setDisabled(readOnly);

        if(readOnly) {
            ui->deleteButton->setToolTip("Daten können auf der CD nicht gelöscht werden.");
        } else {
            ui->deleteButton->setToolTip(nullptr);
        }
    }
}

