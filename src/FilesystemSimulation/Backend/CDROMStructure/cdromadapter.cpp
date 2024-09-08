#include "cdromadapter.h"
#include "qDebug.h"

CDROMAdapter::CDROMAdapter() {}

bool CDROMAdapter::removeFile(std::string path)
{
    qCritical() << "In einer CD-ROM kann nichts gelöscht werden!";
    return false;
}

bool CDROMAdapter::removeFolder(std::string path)
{
    qCritical() << "In einer CD-ROM kann nichts gelöscht werden!";
    return false;
}

void CDROMAdapter::defragDisk()
{
    qCritical() << "In einer CD-ROM kann nicht defragmentiert werden!";
}
