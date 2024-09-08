//
// Created by Tim Preisler on 27.06.24.
//
#define maximum_file_count 10
#include <map>
#include "BsFat.h"

#include <__filesystem/path.h>

#include "qDebug.h"


BsFat *BsFat::createBsFat(size_t plattengroesse, size_t blockgroesse) {
    // Überprüfen, ob die Blockgröße nicht null ist, um eine Division durch null zu vermeiden
    if (blockgroesse == 0) {
        qCritical() << "Fehler: Die Blockgröße darf nicht null sein.";
    } else {


        // Berechnen der Anzahl der Blöcke
        size_t anzahlBloecke = (plattengroesse + blockgroesse - 1) / blockgroesse;

        // Speicher für die BsFat-Struktur allokieren
        auto *bsFat = new BsFat();
        if (bsFat == nullptr) {
            qCritical() << "Fehler: Speicher konnte nicht allokiert werden.";
        } else {

            // Speicher für das Blockstatus-Array allokieren
            bsFat->fileAllocationTable = new FileAllocationTableRow[anzahlBloecke];
            if (bsFat->fileAllocationTable == nullptr) {
                free(bsFat); // BsFat-Struktur freigeben, wenn die Allokation fehlschlägt
                qCritical() << "Fehler: Speicher für Blockstatus-Array konnte nicht allokiert werden.";
            } else {

                bsFat->directoryTableFormat = new BsDtf("root");

                // Initialisieren der BsFat-Struktur
                bsFat->diskSize = plattengroesse;
                bsFat->blockSize = blockgroesse;
                bsFat->blockCount = anzahlBloecke;

                // Initialisieren aller Blöcke als frei
                for (size_t i = 0; i < anzahlBloecke; i++) {
                    bsFat->fileAllocationTable[i].cleanRow();
                }
                bsFat->files = new BsFile[maximum_file_count];
                for (int i = 0; i < maximum_file_count; i++) {
                    bsFat->files[i].set_filename((char *) "\0");
                }

                return bsFat;
            }
        }
    }
}

size_t BsFat::getFreeDiskSpace() {
    if (this->fileAllocationTable == nullptr) {
        return -1; // Fehlerfall
    }
    size_t freeBlocks = 0;
    for (size_t i = 0; i < this->blockCount; i++) {
        if (this->fileAllocationTable[i].getBlockState() == FREE) {
            freeBlocks++;
        }
    }

    return freeBlocks * this->blockSize;
}

void BsFat::freeBsFat() {
    delete[] fileAllocationTable;
    delete[] this;
}

float BsFat::getFragmentation() {
    if (this->get_block_states() == nullptr || this->blockCount == 0) {
        qCritical() << "Fehler: Ungültige Eingabe!";
    } else {

        int contigousBlocks = 0;
        int possibleBlocks = 0;
        for (int i = 0; i < this->blockCount; i++) {
            if (this->get_block_states()[i].getBlockState() == OCCUPIED) {
                possibleBlocks++;
            }
        }


        for (int i = 0; i < maximum_file_count; i++) {
            if (this->files[i].get_filename()[0] != '\0') {
                const BsCluster *fileCluster = this->files[i].get_bs_cluster();
                size_t currentIndex = this->files[i].get_bs_cluster()->get_index();
                while (fileCluster != nullptr) {
                    if (currentIndex == fileCluster->get_index() || currentIndex + 1 == fileCluster->get_index()) {
                        contigousBlocks++;
                    }
                    currentIndex = fileCluster->get_index();
                    fileCluster = fileCluster->get_next();
                }
            }
        }
        float fragmentation = 1 - static_cast<float>(contigousBlocks) / static_cast<float>(possibleBlocks);
        return fragmentation * 100;
    }
}


int BsFat::block_is_occupied(const size_t i) {
    // Wenn der Block durch ein DateiCluster belegt ist, zeigen Sie den Index der Datei an
    bool fileClusterFound = false;
    for (int j = 0; j < maximum_file_count; j++) {
        const BsCluster *currentCluster = this->get_files()[j].get_bs_cluster();
        while (currentCluster != nullptr) {
            if (currentCluster->get_index() == i) {
                return j;
                fileClusterFound = true;
                break;
            }
            currentCluster = currentCluster->get_next();
        }
        if (fileClusterFound) {
            break;
        }
    }
    if (!fileClusterFound) {
        qCritical() << "Fehler: Dateiindex nicht gefunden.";
    }
}

void BsFat::defragDisk(int callCount) {
    if (this->fileAllocationTable == nullptr || this->blockCount == 0) {
        qCritical() << "Fehler: Ungültige Eingabe.";
    } else {

        // Verschiebe die Blöcke, um sie zu defragmentieren
        size_t freeBlockIndex = 0;

        /*Erläuterung: Schiebt alle Blöcke einer File der Reihe nach zum nächsten,
         * freien Block, das sorgt dafür, dass die Blöcke zusammenhängend sind,
         * es sei denn, da ist ein D oder anderes R zwischen.
         * Deswegen wird es dann erneut aufgerufen,
         * aber Maximum 10-mal damit man es dann noch verbessern kann :)*/

        //für alle files ausführen
        for (int i = 0; i < maximum_file_count; i++) {
            //nur für files, die auch gefüllt sind
            if (this->files[i].get_filename()[0] != '\0') {
                BsCluster *fileCluster = this->files[i].get_bs_cluster();
                BsCluster *prevCluster = nullptr;

                while (fileCluster != nullptr) {
                    // Suche nach dem nächsten freien Block
                    while (freeBlockIndex < this->blockCount &&
                           this->get_block_states()[freeBlockIndex].getBlockState() != FREE) {
                        freeBlockIndex++;
                    }

                    if (freeBlockIndex >= this->blockCount) {
                        // std::cerr << "Fehler: Kein freier Block gefunden.\n";
                        // exit(EXIT_FAILURE);
                        qCritical() << "Fehler: Kein freier Block gefunden.";
                    } else {

                        // Verschiebe den Block zu dem freien Blockindex
                        this->fileAllocationTable[freeBlockIndex].setBsCluster(
                            this->fileAllocationTable[fileCluster->get_index()].getBsCluster());
                        this->fileAllocationTable[fileCluster->get_index()].cleanRow();

                        // Aktualisiere den Cluster
                        fileCluster->set_index(freeBlockIndex);

                        // Verbinde die Cluster
                        if (prevCluster != nullptr) {
                            prevCluster->set_next(fileCluster);
                        } else {
                            this->files[i].set_bs_cluster(fileCluster);
                        }

                        prevCluster = fileCluster;
                        fileCluster = fileCluster->get_next();
                        freeBlockIndex++;
                    }
                }

                if (prevCluster != nullptr) {
                    prevCluster->set_next(nullptr);
                }
            }
        }

        if (getFragmentation() > 5 && callCount < 10) {
            defragDisk(callCount++);
        }
    }
}

bool BsFat::set_block_state(int index, BlockState blockstate)
{
    if(index > blockCount - 1 || index < 0){
        return false;
    }
    if(fileAllocationTable[index].getBlockState() != BlockState::OCCUPIED ){
        fileAllocationTable[index].setBlockState(blockstate);
        return true;
    } else {
        qCritical() << "Blockstate of this block is OCCUPIED! This cannot be changed!";
        return false;
    }
}

size_t BsFat::get_system_usage()
{
    size_t not_usage = 0;
    for(int i = 0; i < blockCount; i++){
        if(fileAllocationTable[i].getBlockState() == FREE){
            not_usage += blockSize;
        }
    }

    return (blockSize * blockCount) - not_usage;
}

std::map<int, std::pair<BlockState, int> > BsFat::getBlockStates()
{
    std::map<int, std::pair<BlockState, int>> blockstates;
    if (this->get_block_states() == nullptr || this->blockCount == 0) {
        qCritical() << "Fehler: Ungültige Eingabe.";
    } else {

        for (size_t i = 0; i < this->blockCount; i++) {
            switch (this->get_block_states()[i].getBlockState()) {
            case FREE:
                blockstates.insert(std::make_pair(i, std::make_pair(BlockState::FREE, -1)));
                break;
            case RESERVED:
                blockstates.insert(std::make_pair(i, std::make_pair(BlockState::RESERVED, -1)));
                break;
            case DEFECT:
                blockstates.insert(std::make_pair(i, std::make_pair(BlockState::DEFECT, -1)));
                break;
            case OCCUPIED:
                blockstates.insert(std::make_pair(i, std::make_pair(BlockState::OCCUPIED, block_is_occupied(i))));
                break;
            default:
                break;
            }
        }
        return blockstates;
    }
}
