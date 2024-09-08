#ifndef ADDFILE_H
#define ADDFILE_H

#include <QDialog>
#include "Backend/MBR/MBR.h"

namespace Ui {
class AddTreeItem;
}

class AddTreeItem : public QDialog
{
    Q_OBJECT

public:
    explicit AddTreeItem(QWidget *parent = nullptr);
    ~AddTreeItem();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddTreeItem *ui;
    MBR *master_boot_record;
};

#endif // ADDFILE_H
