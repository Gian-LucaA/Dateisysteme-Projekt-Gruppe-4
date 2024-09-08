#ifndef ADDFOLDER_H
#define ADDFOLDER_H

#include <QDialog>

namespace Ui {
class addFolder;
}

class addFolder : public QDialog
{
    Q_OBJECT

public:
    explicit addFolder(QWidget *parent = nullptr);
    ~addFolder();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::addFolder *ui;
};

#endif // ADDFOLDER_H
