#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QtWidgets/qstackedwidget.h>

#include "Backend/MBR/MBR.h"
#include "TreeModel.h"
#include "TreeItem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    MBR* getMBR();
    void PopulateTree();
    void calculateUsage();
    TreeModel *treeModel;
    int getBlockCount();
    QString getSelectedTreeItemPath();
    bool getReadOnly();
    int getSystem();

private slots:
    void on_addItems_clicked();
    void on_inodeButton_clicked();
    void on_fatButton_clicked();
    void on_treeView_clicked(const QModelIndex &index);
    void InitializeTree(MBR* master_boot_record);
    void on_pushButton_clicked();
    void on_showFragmentation_clicked();
    void on_backButton_clicked();
    void on_fatButton_CD_clicked();
    void on_deleteButton_clicked();
    void on_stackedWidget_currentChanged(int arg1);

    void on_copyButton_clicked();

private:
    Ui::MainWindow *ui;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    void setupBlockGrid();
    QStackedWidget *stackedWidget;
    MBR* master_boot_record;
    TreeItem* rootItem;
    QString selectedTreeItemPath;
    bool readOnly;
    int selctedSystem;
};

#endif // MAINWINDOW_H
