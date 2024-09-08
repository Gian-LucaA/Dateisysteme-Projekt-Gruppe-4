#include "globalerrormessagehandler.h"
#include "Toast.h"
#include <QApplication>
#include <QDebug>

void globalMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QString formattedMessage;

    switch (type) {
    case QtDebugMsg:
        formattedMessage = QString("Debug: %1").arg(msg);
        break;
    case QtInfoMsg:
        formattedMessage = QString("Info: %1").arg(msg);
        break;
    case QtWarningMsg:
        formattedMessage = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        formattedMessage = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        formattedMessage = QString("Fatal: %1").arg(msg);
        abort(); // Bei einem fatalen Fehler wird das Programm abgebrochen
    }

    // Anzeigen des Fehlers als Toast
    QWidget *mainWindow = QApplication::activeWindow(); // Das aktive Fenster finden
    if (mainWindow) {
        Toast *toast = new Toast(mainWindow);
        toast->showMessage(formattedMessage, 10000); // Toast für 10 Sekunden anzeigen
    }

    // Auch im Standard-Output anzeigen
    fprintf(stderr, "%s\n", formattedMessage.toLocal8Bit().constData());
}
