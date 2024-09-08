#ifndef DELETEDIALOG_H
#define DELETEDIALOG_H

#include "Backend/MBR/MBR.h"
#include "mainwindow.h"
#include <QDialog>

namespace Ui {
class deleteDialog;
}

class deleteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit deleteDialog(QWidget *parent = nullptr);
    ~deleteDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::deleteDialog *ui;
    MBR* master_boot_record;
    QString selectedTreeItemPath;
    MainWindow* parentMainWindow;
};

#endif // DELETEDIALOG_H
