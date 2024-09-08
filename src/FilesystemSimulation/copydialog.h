#ifndef COPYDIALOG_H
#define COPYDIALOG_H

#include "Backend/MBR/MBR.h"
#include "mainwindow.h"
#include "ui_deletedialog.h"
#include <QDialog>

namespace Ui {
class CopyDialog;
}

class CopyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CopyDialog(QWidget *parent = nullptr);
    ~CopyDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CopyDialog *ui;
    MBR* master_boot_record;
    QString selectedTreeItemPath;
    MainWindow* parentMainWindow;
    int selectedSystem;
};

#endif // COPYDIALOG_H
