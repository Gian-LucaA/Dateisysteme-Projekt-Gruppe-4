#include <QtGui/qvalidator.h>
#include <iostream>

#include "addfolder.h"
#include "ui_addfolder.h"
#include "mainwindow.h"

addFolder::addFolder(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addFolder)
{
    ui->setupUi(this);

    setWindowTitle("Ordner");

    // Pfad Validieren
    QRegularExpression regex("^[A-Za-z0-9/]*$");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->folderName->setValidator(validator);
}

addFolder::~addFolder()
{
    delete ui;
}

void addFolder::on_buttonBox_accepted()
{

    ui->folderName->validator();
    QString path = ui->folderName->text();

    if (path.startsWith('/')) {
        path.remove(0, 1);
    }
    if (!path.endsWith('/')) {
        path.append('/');
    }

    MainWindow *parentMainWindow = qobject_cast<MainWindow*>(parentWidget());
    MBR* master_boot_record = parentMainWindow->getMBR();

    std::string fullPathString = "/root/" + path.toStdString();

    if(master_boot_record->get_system()->createFolder(path.toStdString())) {
        parentMainWindow->treeModel->addStringPathToTree(fullPathString, true);
    }
}

