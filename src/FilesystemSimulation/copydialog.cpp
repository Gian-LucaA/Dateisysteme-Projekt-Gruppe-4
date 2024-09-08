#include "copydialog.h"
#include "ui_copydialog.h"

CopyDialog::CopyDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CopyDialog)
{
    ui->setupUi(this);

    setWindowTitle("Kopieren");

    parentMainWindow = qobject_cast<MainWindow*>(parentWidget());
    master_boot_record = parentMainWindow->getMBR();
    selectedTreeItemPath = parentMainWindow->getSelectedTreeItemPath();
    selectedSystem = parentMainWindow->getSystem();

    // Pfad Validieren
    QRegularExpression regex("^[A-Za-z0-9/]*$");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->destination_path->setValidator(validator);

    ui->source_path->setText("/" + selectedTreeItemPath);

    if(selectedSystem != 0)
        ui->destination->addItem("FAT");

    if(selectedSystem != 1)
        ui->destination->addItem("INode");

    if(selectedSystem != 2)
        ui->destination->addItem("CD-Rom");
}

CopyDialog::~CopyDialog()
{
    delete ui;
}

void CopyDialog::on_buttonBox_accepted()
{
    QString destintaionPath = ui->destination_path->text();
    QString sourcePath = ui->source_path->text();
    QString destinationSystem = ui->destination->currentText();
    int destinationSystemID;

    if(!QString::compare(destinationSystem, "FAT", Qt::CaseSensitive)){
        destinationSystemID = 0;
    } else if(!QString::compare(destinationSystem, "INode", Qt::CaseSensitive)){
        destinationSystemID = 1;
    } else {
        destinationSystemID = 2;
    }

    if (destintaionPath.startsWith('/')) {
        destintaionPath.remove(0, 1);
    }
    if (!destintaionPath.endsWith('/')) {
        destintaionPath.append('/');
    }

    std::string destinationPathString = destintaionPath.toStdString();
    std::string sourcePathString = sourcePath.toStdString();

    FileInformationInterface* fileToCopy = master_boot_record->get_system()->getFile(sourcePathString);

    if (master_boot_record->get_system()->copyFileToFileSystem(destinationPathString, fileToCopy, master_boot_record->get_system(destinationSystemID))) {
        qInfo("Datei wurde erfolgreich kopiert.");
    }
}

