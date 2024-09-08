#include "mainwindow.h"
#include "globalerrormessagehandler.h"
#include "Toast.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <exception>

int main(int argc, char *argv[])
{
    try {
        QApplication a(argc, argv);

        // Installiere den globalen Error-Handler für Qt-Nachrichten
        qInstallMessageHandler(globalMessageHandler);

        QTranslator translator;
        const QStringList uiLanguages = QLocale::system().uiLanguages();
            for (const QString &locale : uiLanguages) {
            const QString baseName = "FilesystemSimulation_" + QLocale(locale).name();
            if (translator.load(":/i18n/" + baseName)) {
                a.installTranslator(&translator);
                break;
            }
        }

        MainWindow w;
        w.show();
        return a.exec();
    } catch (const std::exception &ex) {
        // Fängt alle Ausnahmen vom Typ std::exception ab
        QWidget *mainWindow = QApplication::activeWindow();
        if (mainWindow) {
            Toast *toast = new Toast(mainWindow);
            toast->showMessage(QString("Exception: %1").arg(ex.what()), 5000); // Zeige Toast für 5 Sekunden
        }

        // Zeige zur Sicherheit auch eine MessageBox
        QMessageBox::critical(nullptr, "Kritischer Fehler", QString("Ein unerwarteter Fehler ist aufgetreten: %1").arg(ex.what()));
    } catch (...) {
        // Fängt alle anderen, nicht standardisierten Ausnahmen ab
        QWidget *mainWindow = QApplication::activeWindow();
        if (mainWindow) {
            Toast *toast = new Toast(mainWindow);
            toast->showMessage("Unbekannte Ausnahme aufgetreten!", 5000); // Zeige Toast für 5 Sekunden
        }

        // Zeige zur Sicherheit auch eine MessageBox
        QMessageBox::critical(nullptr, "Kritischer Fehler", "Eine unbekannte Ausnahme ist aufgetreten!");
    }
}
