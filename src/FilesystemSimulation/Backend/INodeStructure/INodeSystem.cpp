#include "INodeSystem.h"
#include "qdebug.h"
#include "../../helpers/PathHelper.h"
#include "Data/DataHandler.h"

using namespace::std;


INodeSystem *INodeSystem::createSystem(size_t diskSize, size_t blockSize) {
    // Überprüfen, ob die Blockgröße nicht null ist, um eine Division durch null zu vermeiden
    if (blockSize == 0) {
        qCritical() << "Fehler: Die Blockgröße darf nicht null sein.";
    } else {
        auto *iNodeSystem = new INodeSystem();
        if (iNodeSystem == nullptr) {
            qCritical() << "Fehler: Speicher konnte nicht allokiert werden.";
        } else {
            // Ermittlung der Blockanzahl anhand der Festplatten- und Blockgröße
            size_t blockCount = (diskSize + blockSize - 1) / blockSize;
            iNodeSystem->data_blocks = new DataBlock[blockCount];
            if (iNodeSystem->data_blocks == nullptr) {
                free(iNodeSystem); // INodeSystem-Struktur freigeben, wenn die Allokation fehlschlägt.
                qCritical() << "Fehler: Speicher für das DatenBlock-Array konnte nicht allokiert werden.";
            } else {
                iNodeSystem->diskSize = diskSize;
                iNodeSystem->blockSize = blockSize;
                iNodeSystem->blockCount = blockCount;
                iNodeSystem->iNodes.push_back(INode(iNodeSystem->nextINodeNumber++, INodeType::DIRECTORY));

                // Initialisieren aller Blöcke als frei
                for (size_t i = 0; i < blockCount; i++) {
                    iNodeSystem->data_blocks[i] = DataBlock(BlockState::FREE);
                }

                return iNodeSystem;
            }
        }
    }
    return nullptr;
}

void INodeSystem::setNewSizeToFolder(string path, int fileSize) {
    vector<string> split = PathHelper::split(path, '/');
    //Ermittelt root Datei
    INode *dir = this->getRootDir();
    for (int i = 0; i < split.size() - 1; i++) {
        int nextIndex = dir->directoryEntries[split.at(i)];
        dir = getINodeByIndex(nextIndex);
        INodeMetadata metadata = dir->get_metadata();
        size_t newSize = metadata.get_size() + fileSize;
        dir->metadata.size = newSize;
    }
}

void INodeSystem::addFileToDirectory(string path, int iNodeFileNumber) {
    INode *dir = getInodeDirFromPath(path);
    //Überprüfung auf vorhanden sein des Ordners
    if (dir != nullptr) {
        string fileName = PathHelper::split(path, '/').back();
        dir->directoryEntries[fileName] = iNodeFileNumber;
    } else {
        qCritical() << "INode doesn't found!";
    }
}

void INodeSystem::getDirectoryTree(std::vector<std::string> *paths, std::string currentPath, INode* currentINode) {
    //Zur Speicherung von Ordnern, da diese nach und nach abgearbeitet werden
    std::vector<INode*> dirs;

    currentPath = currentPath + getNameByIndex(currentINode->get_index()) + "/";
    for (const auto &pair: currentINode->directoryEntries) {
        // Überprüfung, ob der aktuelle Index nicht zu einer Datei gehört
        if (!isFile(pair.second)) {
            INode *folder = getINodeByIndex(pair.second);
            if (folder != nullptr) {
                dirs.push_back(folder);
            }
        } else {
            paths->push_back(currentPath + pair.first);
        }
    }
    for (INode *folder: dirs) {
        paths->push_back(currentPath + getNameByIndex(folder->get_index()) + "/");
        getDirectoryTree(paths, currentPath, folder);
    }
}

int INodeSystem::block_is_occupied(const size_t data_block_index) {
    // Wenn der Block durch ein DateiCluster belegt ist, zeigen Sie den Index der Datei an
    for (int j = 0; j < this->iNodes.size(); j++) {
        INode node = iNodes.at(j);
        if (node.isFile()) {
            //Ermittlung aller von der INode belegten Blocknummern
            vector<int> blockNumbers = node.getAllUsedBlockNumbers();
            //Iterierung durch die Blocknummern zur Verprobung auf vorhanden sein des zu suchenden Blocks
            for (int index = 0; index < blockNumbers.size(); ++index) {
                if (data_block_index == blockNumbers.at(index)) {
                    return node.get_index();
                }
            }
        }
    }
    return -1;
}

void INodeSystem::defragDisk(int callCount) {
    // Verschiebe die Blöcke, um sie zu defragmentieren
    size_t freeBlockIndex = 0;

    /*Erläuterung: Schiebt alle Blöcke eines File der Reihe nach zum nächsten, freien Block, das sorgt dafür,
     * dass die Blöcke zusammenhängend sind, es sei denn, da ist ein D oder anderes R zwischen. Deswegen wird
     * es dann erneut aufgerufen, aber Maximum 10-mal damit man es dann noch verbessern kann :)*/
    //für alle Files ausführen
    for (int i = 0; i < iNodes.size(); i++) {
        INode node = iNodes.at(i);
        //Nur ausführen, wenn es sich um eine Datei handelt
        if (node.isFile()) {
            vector<int> used_blocks = node.getAllUsedBlockNumbers();
            for (int j = 0; j < used_blocks.size(); j++) {
                // Suche nach dem nächsten freien Block
                while (freeBlockIndex < this->blockCount &&
                       this->data_blocks[freeBlockIndex].get_block_state() != FREE) {
                    freeBlockIndex++;
                }

                if (freeBlockIndex >= this->blockCount) {
                    qCritical() << "Fehler: Kein freier Block gefunden.\n";
                }

                this->data_blocks[freeBlockIndex] = data_blocks[used_blocks.at(j)];
                data_blocks[used_blocks.at(j)].clearData();
                DataHandler::setDataBlockIndex(&node, j, freeBlockIndex, this);

                freeBlockIndex++;
            }
        }
    }

    if (getFragmentation() > 5 && callCount < 10) {
        defragDisk(++callCount);
    }
}

float INodeSystem::getFragmentation() {
    int contigousBlocks = 0;
    int possibleBlocks = 0;
    for (int i = 0; i < this->blockCount; i++) {
        if (this->data_blocks[i].get_block_state() == OCCUPIED) {
            possibleBlocks++;
        }
    }

    for (int i = 0; i < iNodes.size(); i++) {
        INode node = iNodes.at(i);
        if (node.metadata.type == INodeType::FILE) {
            contigousBlocks++;
            vector<int> used_datablocks = node.getAllUsedBlockNumbers();
            if (used_datablocks.size() != 1) {
                for (int j = 0; j < used_datablocks.size() - 1; j++) {
                    if (used_datablocks.at(j) + 1 == used_datablocks.at(j + 1)) {
                        contigousBlocks++;
                    }
                }
            }
        }
    }
    float fragmentation = 1 - (possibleBlocks > 0 ? (static_cast<float>(contigousBlocks) / static_cast<float>(possibleBlocks)) : 1);
    return fragmentation * 100;
}

bool INodeSystem::isFile(int iNodeNumber) {
    INode *node = this->getINodeByIndex(iNodeNumber);
    return (node != nullptr) && node->get_metadata().get_type() == INodeType::FILE;
}

int INodeSystem::getFreeBlockCount() {
    int freeBlockCount = 0;
    for (int i = 0; i < this->blockCount; i++) {
        if (this->data_blocks[i].get_block_state() == BlockState::FREE) {
            freeBlockCount++;
        }
    }
    return freeBlockCount;
}

INode *INodeSystem::getInodeDirFromPath(string path) {
    char delimiter = '/';
    std::vector<std::string> result = PathHelper::split(path, delimiter);

    INode *dir = this->getRootDir();

    //soll letzes element herauswerfen...
    //FileName herausnehen
    if (result.at(result.size() - 1).find('.') != std::string::npos) {
        result.pop_back();
    }

    result.erase(
        std::remove_if(result.begin(), result.end(), [](const std::string &str) {
            return str.empty();
        }),
        result.end()
    );

    for (const auto &part: result) {
        int iNodeNumber = getIndexByName(part, dir->directoryEntries);
        dir = getINodeByIndex(iNodeNumber);
        if (dir == nullptr) {
            break;
        }
    }
    return dir;
}

INode *INodeSystem::getInodeFileFromPath(string path) {
    INode *dir = getInodeDirFromPath(path);
    if (dir != nullptr) {
        string fileName = PathHelper::split(path, '/').back();
        for (auto &pair: dir->directoryEntries) {
            if (pair.first == fileName) {
                return getINodeByIndex(pair.second);
            }
        }
    }
    return nullptr;
}

INode *INodeSystem::getINodeByIndex(int searchIndex) {
    for (INode &node: iNodes) {
        if (node.get_index() == searchIndex) {
            return &node;
        }
    }
    return nullptr;
}

std::string INodeSystem::getNameByIndex(int index) {
    for (const INode &node: iNodes) {
        if (node.get_metadata().get_type() == INodeType::DIRECTORY) {
            for (const auto &pair: node.directoryEntries) {
                if (pair.second == index) {
                    return pair.first;
                }
            }
        }
    }
    return "";
}

int INodeSystem::getIndexByName(string name) {
    for (const INode &node: iNodes) {
        if (node.get_metadata().get_type() == INodeType::DIRECTORY) {
            for (const auto &pair: node.directoryEntries) {
                if (pair.first == name) {
                    return pair.second;
                }
            }
        }
    }
    return -1;
}

int INodeSystem::getIndexByName(string name, std::unordered_map<string, int> directory_entries) {
    for (const auto &pair: directory_entries) {
        INode *node = getINodeByIndex(pair.second);
        if (node != nullptr && pair.first == name)
            return pair.second;
    }
    return -1;
}

INode* INodeSystem::getRootDir(){
    return &iNodes.at(0);
}
