//
// Created by Tim Preisler on 27.06.24.
//
#define maximum_file_count 10 //TODO ändern
#include "BsFile.h"

#include <sstream>

#include "../../helpers/DateTimeHelper.h"

#include "qDebug.h"

bool BsFile::createFile(BsFat *pFat, size_t szFile, std::string filename, std::string fileend,
                           std::string filekind,
                           std::string permissions,
                           std::string owner,
                           long long modification_time,
                           long long creation_time) {
    if (pFat == nullptr) {
        qCritical() << "Fehler: Ungültige Eingabe.";
        return false;
    } else {
        size_t freeSpace = pFat->getFreeDiskSpace();
        if (freeSpace <= (size_t) szFile) {
            qWarning() << "Fehler: Nicht genügend freier Speicherplatz verfügbar.";
            return false;
        } else {

            int fileIndex = -1;
            for (int i = 0; i < maximum_file_count; i++) {
                if (pFat->get_files()[i].filename[0] == '\0') {
                    fileIndex = i;
                    break;
                }
            }

            if (fileIndex == -1) {
                qCritical() << "Fehler: Kein freier Platz für neue Datei verfügbar.";
                return false;
            } else {



                std::vector<std::string> filePath = BsDtf::split(filename, '/');
                pFat->get_files()[fileIndex].path = filename;
                pFat->get_files()[fileIndex].filename = filePath[filePath.size() - 1];
                pFat->get_files()[fileIndex].filePrefix = fileend;
                pFat->get_files()[fileIndex].filekind = filekind;
                pFat->get_files()[fileIndex].file_permissions = permissions;
                pFat->get_files()[fileIndex].file_owner = owner;
                pFat->get_files()[fileIndex].szFile = szFile;
                pFat->get_files()[fileIndex].file_creation_time = modification_time;
                pFat->get_files()[fileIndex].file_modification_time = creation_time;

                pFat->get_files()[fileIndex].bsCluster = nullptr;

                int blocksNeeded = (int) ((szFile + pFat->get_block_size() - 1) / pFat->get_block_size());
                int blocksReserved = 0;
                srand((unsigned int) time(nullptr));
                for (int i = 0; blocksReserved < blocksNeeded; i < pFat->get_block_count() ? i++ : 0) {
                    if (pFat->get_block_states()[i].getBlockState() == FREE) {
                        // if (rand() % 2) {
                            if (pFat->get_files()[fileIndex].bsCluster == nullptr) {
                                pFat->get_files()[fileIndex].bsCluster = new BsCluster();
                                if (pFat->get_files()[fileIndex].bsCluster == nullptr) {
                                    qCritical() << "Fehler: Speicher für bsCluster konnte nicht allokiert werden.";
                                    return false;
                                }
                                pFat->get_files()[fileIndex].bsCluster->set_index(i);
                                pFat->get_block_states()[i].setBsCluster(*pFat->get_files()[fileIndex].bsCluster);
                            } else {
                                pFat->get_block_states()[i].setBsCluster(
                                    BsCluster::appendBsCluster(pFat->get_files()[fileIndex].bsCluster, i));
                            }
                            blocksReserved++;
                        // }
                    }
                }

                pFat->get_dtf()->insertFile(filename, &pFat->get_files()[fileIndex]);

                return true;
            }

        }
    }
}

bool BsFile::deleteFile(const BsFat *pFat, std::string fileName) {
    int fileIndex = -1;
    for (int i = 0; i < maximum_file_count; i++) {
        if (pFat->get_files()[i].filename == fileName) {
            fileIndex = i;
            break;
        }
    }

    if (fileIndex == -1) {
        qCritical() << "Fehler: Datei konnte nicht gefunden werden.";
        return false;
    } else {
        while (pFat->get_files()[fileIndex].bsCluster->get_prev() != nullptr) {
            pFat->get_files()[fileIndex].bsCluster = pFat->get_files()[fileIndex].bsCluster->get_prev();
        }
        while (pFat->get_files()[fileIndex].bsCluster != nullptr) {
            pFat->get_block_states()[pFat->get_files()[fileIndex].bsCluster->get_index()].cleanRow();
            BsCluster *next = pFat->get_files()[fileIndex].bsCluster->get_next();
            free(pFat->get_files()[fileIndex].bsCluster);
            pFat->get_files()[fileIndex].bsCluster = next;
        }

        pFat->get_files()[fileIndex].filename = "\0";
        pFat->get_files()[fileIndex].filePrefix = "\0";
        pFat->get_files()[fileIndex].filekind = "\0";
    }
    return true;
}

size_t BsFile::get_file_size(BsFat *pFat) {
    return szFile;
}

FatFileInformation *BsFile::get_file_information(BsFat *pFat) {
    return new FatFileInformation(get_filename(), get_filekind(), get_file_prefix(), get_file_permissions(),
                                  get_file_owner(), get_file_modification_time(), get_file_creation_time(),
                                  get_file_size(pFat));
}
