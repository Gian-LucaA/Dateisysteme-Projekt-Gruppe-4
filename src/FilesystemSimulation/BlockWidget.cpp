#include "BlockWidget.h"

BlockWidget::BlockWidget(QWidget *parent)
    : QWidget(parent), label(new QLabel(this)), layout(new QVBoxLayout(this)) {
    layout->addWidget(label);
    setLayout(layout);

    // Set default size for the widget
    setFixedSize(40, 40);
}

void BlockWidget::setStatus(BlockState status, int number) {
    updateAppearance(status);
    label->setText(status == OCCUPIED ? QString::number(number) : (status == FREE ? "F" : (status == RESERVED ? "R" : "X")));
}

void BlockWidget::updateAppearance(BlockState status) {
    QString backgroundColor;
    QString textColor;

    switch (status) {
    case OCCUPIED:
        backgroundColor = "#3a5f9d"; // blue
        textColor = "white";
        break;
    case FREE:
        backgroundColor = "#4a9d4e"; // green
        textColor = "white";
        break;
    case RESERVED:
        backgroundColor = "#9d9d9d"; // gray
        textColor = "black";
        break;
    case DEFECT:
        backgroundColor = "#d7735d"; // red
        textColor = "white";
        break;
    }

    setStyleSheet(QString("background-color: %1;").arg(backgroundColor));
    label->setStyleSheet(QString("color: %1;").arg(textColor));
    label->setAlignment(Qt::AlignCenter);
}

void BlockWidget::mousePressEvent(QMouseEvent *event) {
    emit blockClicked(blockId);
    QWidget::mousePressEvent(event);
}

void BlockWidget::setBlockId(int id) {
    blockId = id;
}
