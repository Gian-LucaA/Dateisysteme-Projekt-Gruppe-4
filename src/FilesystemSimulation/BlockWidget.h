#ifndef BLOCKWIDGET_H
#define BLOCKWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include "Backend/BSFatStructure/BlockState.h"

class BlockWidget : public QWidget {
    Q_OBJECT

public:
    explicit BlockWidget(QWidget *parent = nullptr);
    void setStatus(BlockState status, int number = 0);
    int getBlockId() const;
    void setBlockId(int id);

signals:
    void blockClicked(int blockId);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QLabel *label;
    QVBoxLayout *layout;
    void updateAppearance(BlockState status);
    int blockId;
};

#endif // BLOCKWIDGET_H
