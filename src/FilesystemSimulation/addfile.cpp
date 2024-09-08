
#include "addfile.h"
#include "helpers/PathHelper.h"
#include "mainwindow.h"
#include "ui_addfile.h"

AddTreeItem::AddTreeItem(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddTreeItem)
{
    ui->setupUi(this);

    setWindowTitle("Datei");

    // Pfad Validieren
    QRegularExpression regex("^[A-Za-z0-9/]*$");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit->setValidator(validator);

    // Datei Namen Validieren
    QRegularExpression regex2("^[A-Za-z0-9.]*$");
    QRegularExpressionValidator *validator2 = new QRegularExpressionValidator(regex2, this);
    ui->lineEdit_2->setValidator(validator2);
}

AddTreeItem::~AddTreeItem()
{
    delete ui;
}

void AddTreeItem::on_buttonBox_accepted()
{    
    ui->lineEdit->validator();

    QString path = ui->lineEdit->text();
    QString fileName = ui->lineEdit_2->text();
    QString type = ui->comboBox->currentText();

    if (path.startsWith('/')) {
        path.remove(0, 1);
    }
    if (!path.endsWith('/')) {
        path.append('/');
    }

    std::string pathString = path.toStdString();
    std::string fileNameString = PathHelper::removeFileType(fileName.toStdString());
    std::string typeString = PathHelper::removeAfter(type.toStdString(), ' ');
    std::string fileEndingPath = PathHelper::extractFileEndingFromOption(type.toStdString());
    int fileSize = ui->spinBox->value();

    std::string fullPathString = pathString + fileNameString + fileEndingPath;

    MainWindow *parentMainWindow = qobject_cast<MainWindow*>(parentWidget());
    MBR* master_boot_record = parentMainWindow->getMBR();

    if (master_boot_record->get_system()->createFile(fileSize, fullPathString, fileEndingPath.erase(0, 1), typeString, "TBD", "TBD")){
        parentMainWindow->treeModel->addStringPathToTree(fullPathString, false);
        parentMainWindow->calculateUsage();
    }
}

