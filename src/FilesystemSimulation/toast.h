#ifndef TOAST_H
#define TOAST_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>

class Toast : public QWidget {
    Q_OBJECT

public:
    explicit Toast(QWidget *parent = nullptr);
    void showMessage(const QString &message, int timeout = 2000);

private slots:
    void hideToast();

private:
    QLabel *messageLabel;
    QPropertyAnimation *animation;
};

#endif // TOAST_H
