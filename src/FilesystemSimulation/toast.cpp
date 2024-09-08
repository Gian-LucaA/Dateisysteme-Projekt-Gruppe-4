#include "toast.h"
#include <QVBoxLayout>
#include <QTimer>
#include <QtWidgets/qapplication.h>

Toast::Toast(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_ShowWithoutActivating);

    // Layout und Label
    QVBoxLayout *layout = new QVBoxLayout(this);
    messageLabel = new QLabel(this);
    messageLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(messageLabel);
    layout->setContentsMargins(10, 10, 10, 10);
    setLayout(layout);

    // Appearance
    setStyleSheet("background-color: rgba(0, 0, 0, 180); color: white; border-radius: 10px;");

    // Animation
    animation = new QPropertyAnimation(this, "windowOpacity", this);
    animation->setDuration(300);
}

void Toast::showMessage(const QString &message, int timeout) {
    messageLabel->setText(message);
    adjustSize();
    move(parentWidget()->geometry().center() - rect().center());

    // Fade-in
    setWindowOpacity(0.0);
    show();
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start();

    // Warten
    QTimer::singleShot(timeout, this, &Toast::hideToast);
}

void Toast::hideToast() {
    // Fade-out
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    animation->start();

    // Zuerst alte aufräumen, dann neue zeigen.
    disconnect(animation, &QPropertyAnimation::finished, this, &Toast::close);
    connect(animation, &QPropertyAnimation::finished, this, &Toast::close);
}
