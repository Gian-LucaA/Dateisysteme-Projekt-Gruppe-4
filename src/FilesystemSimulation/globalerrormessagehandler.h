#ifndef GLOBALERRORHANDLER_H
#define GLOBALERRORHANDLER_H

#include <QtGlobal>
#include <QString>

void globalMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // GLOBALERRORHANDLER_H
