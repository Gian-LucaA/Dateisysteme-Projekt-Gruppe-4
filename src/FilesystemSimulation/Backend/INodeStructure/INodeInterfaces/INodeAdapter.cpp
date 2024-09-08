#include "../INodeInterfaces/INodeAdapter.h"
#include "../INode.h"
#include "QDebug"

#include "INodeFileInformation.h"
#include "INodeFolderInformation.h"
#include "../../../helpers/DateTimeHelper.h"
#include "../../../helpers/PathHelper.h"
#include "../Data/DataHandler.h"

using namespace::std;

void INodeAdapter::createSystem(size_t diskSize, size_t sizeBlock) {
    inode_system = INodeSystem::createSystem(diskSize, sizeBlock);
}

bool INodeAdapter::createFile(size_t szFile, std::string path, std::string fileend, std::string filekind,
                              std::string permissions,
                              std::string owner) {
    return createFile(szFile, path, fileend, filekind, permissions, owner, DateTimeHelper::getCurrentDateTimeMS(),
                      DateTimeHelper::getCurrentDateTimeMS());
}

bool INodeAdapter::createFile(size_t szFile, string path, string fileend, string filekind, string permissions,
                              string owner, long long modification_time, long long creation_time) {
    //freien speicher abfragen bevor erstellung
    int blocks_needed = (szFile + inode_system->blockSize - 1) / inode_system->blockSize;
    if (inode_system->getFreeBlockCount() < blocks_needed) {
        qWarning() << "Fehler: Nicht genügend freier Speicherplatz verfügbar.";
        return false;
    }

    if(inode_system->getInodeFileFromPath(path) != nullptr){
        qWarning() << "Fehler: Datei exsistiert bereits.";
        return false;
    }

    if(inode_system->getInodeDirFromPath(path) == nullptr){
        createFolder(PathHelper::getDirectoryPath(path, '/'));
    }

    int inodeNumber = inode_system->nextINodeNumber++;
    INode inode(inodeNumber, INodeType::FILE, fileend, filekind, owner, permissions,
                szFile, modification_time, creation_time);
    DataHandler::fillDataBlocks(&inode, inode_system);
    inode_system->iNodes.push_back(inode);
    inode_system->addFileToDirectory(path, inodeNumber);
    inode_system->setNewSizeToFolder(path, szFile);

    return true;
}

bool INodeAdapter::createFolder(std::string path) {
    //besorgt sich anhand des Pfades einen Order, da ein noch nicht exsitierender Ordner ein Null liefert
    INode *latestDir = inode_system->getInodeDirFromPath(path);
    if (latestDir == nullptr) {
        std::vector<std::string> result = PathHelper::split(path, '/');
        latestDir = inode_system->getRootDir();
        for (const auto &part: result) {
            //verprobt auf vorhandensein des aktuellen Ordners
            int iNodeNumber = inode_system->getIndexByName(part, latestDir->directoryEntries);
            if (iNodeNumber == -1) {
                int newInodeNumber = inode_system->nextINodeNumber++;
                latestDir->directoryEntries[part] = newInodeNumber;
                INode dir(newInodeNumber, INodeType::DIRECTORY);
                inode_system->iNodes.push_back(dir);
                latestDir = &inode_system->iNodes.back();
            } else {
                latestDir = inode_system->getINodeByIndex(iNodeNumber);
            }
        }
    } else {
        qWarning() << "Ordner existiert bereits.";
        return false;
    }
    return true;
}

FileInformationInterface *INodeAdapter::getFile(std::string path) {
    INode *iNode = inode_system->getInodeFileFromPath(path);
    int index = iNode->get_index();
    string name = inode_system->getNameByIndex(index);

    INodeMetadata metadata = iNode->get_metadata();

    return new INodeFileInformation(name,
                                    metadata.get_fileKind(),
                                    metadata.get_fileEnd(),
                                    metadata.get_permissions(),
                                    metadata.get_owner(),
                                    metadata.get_modification_time(),
                                    metadata.get_creation_time(),
                                    metadata.get_size());
}

FolderInformationInterface *INodeAdapter::getFolder(std::string path) {
    INode *iNode = inode_system->getInodeDirFromPath(path);
    string name = inode_system->getNameByIndex(iNode->get_index());

    return new INodeFolderInformation(name, iNode->get_metadata().get_size());
}

bool INodeAdapter::removeFile(std::string path) {
    vector<string> split = PathHelper::split(path, '/');
    string fileName = split.back();
    int index = inode_system->getIndexByName(fileName);
    INode *dir;
    if(inode_system->isFile(index)){
         dir = inode_system->getInodeDirFromPath(path);
    }else{
        split.pop_back();
        dir = inode_system->getInodeDirFromPath(PathHelper::join(split, '/'));
    }

    if(dir == nullptr){
        qWarning() << "Datei mit dem Pfad: " << path << " nicht gefunden.";
        return false;
    }

    //Die Verzweigung der Datei wird aus dem Oberordner entnommen
    dir->directoryEntries.erase(fileName);

    auto it = std::find_if(inode_system->iNodes.begin(), inode_system->iNodes.end(), [index](const INode &node) {
        return node.get_index() == index;
    });

    if (it != inode_system->iNodes.end()) {
        if(it->isFile()){
            vector<int> blockNumbers = it->getAllUsedBlockNumbers();
            for (int i: blockNumbers) {
                inode_system->data_blocks[i].clearData();
            }
            inode_system->setNewSizeToFolder(path, -it->get_metadata().get_size());
        }
        inode_system->iNodes.erase(it);
        return true;
    }
    return false;
}

bool INodeAdapter::removeFolder(string path) {
    vector<string> filesToRemove;

    INode* dir = inode_system->getInodeDirFromPath(path);
    vector<string> split = PathHelper::split(path, '/');
    string dirName = split.back();
    split.pop_back();
    string newPath = PathHelper::join(split, '/');
    filesToRemove.push_back(path);
    inode_system->getDirectoryTree(&filesToRemove, newPath + "/", dir);
    std::reverse(filesToRemove.begin(), filesToRemove.end());

    for(string &file : filesToRemove){
        if(!removeFile(file)){
            return false;
        }
    }
    INode *dir_to_delete = inode_system->getInodeDirFromPath(newPath);
    if(dir_to_delete == nullptr){
        qCritical() << "INode konnte nicht gefunden werden.";
        return false;
    }
    return true;
}

void INodeAdapter::defragDisk() {
    inode_system->defragDisk(0);
}

float INodeAdapter::getFragmentation() {
    return inode_system->getFragmentation();
}

std::vector<std::string> INodeAdapter::getDirectoryTree() {
    INode* root = inode_system->getRootDir();
    std::vector<std::string> directory_tree;
    inode_system->getDirectoryTree(&directory_tree, "/root", root);
    return directory_tree;
}

void INodeAdapter::clearFileSystem() {
    for (int i = 0; i < inode_system->iNodes.size(); ++i) {
        inode_system->iNodes.at(i).directoryEntries.clear();
    }
    inode_system->iNodes.clear();
    delete[] inode_system->data_blocks;
}

std::map<int, std::pair<BlockState, int> > INodeAdapter::getBlockStates() {

    std::map<int, std::pair<BlockState, int>> blockstates;
    if (inode_system->data_blocks == nullptr || inode_system->blockCount == 0) {
        // std::cerr << "Fehler: Ungültige Eingabe.\n";
        // exit(EXIT_FAILURE);
        qCritical() << "Fehler: Ungültige Eingabe.";
    } else {

        for (size_t i = 0; i < inode_system->blockCount; i++) {
            switch (inode_system->data_blocks[i].get_block_state()) {
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
                blockstates.insert(std::make_pair(i, std::make_pair(BlockState::OCCUPIED, inode_system->block_is_occupied(i))));
                break;
            default:
                break;
            }
        }
    }
    return blockstates;
}

bool INodeAdapter::setBlock(int blockIndex, BlockState blockstate) {
    if(blockIndex < 0 || inode_system->blockCount - blockIndex < 0){
        qWarning() << "Fehler: Blockindex nicht vorhanden.";
        return false;
    }
    inode_system->data_blocks[blockIndex].set_block_state(blockstate);
    return true;
}

size_t INodeAdapter::getPartitionUsage() {
    int blocks_free = 0;
    for (int i = 0; i < inode_system->blockCount; ++i) {
        if (inode_system->data_blocks[i].get_block_state() == BlockState::FREE) {
            blocks_free++;
        }
    }
    return inode_system->diskSize - (blocks_free * inode_system->blockSize);
}

bool INodeAdapter::copyFileToFileSystem(string pathToCopy, FileInformationInterface *fileInformations,
                                        FileSystemInterface *fileSystem) {
    return fileSystem->createFile(
        fileInformations->get_file_size(),
        pathToCopy + fileInformations->get_file_name(),
        fileInformations->get_file_prefix(),
        fileInformations->get_file_kind(),
        fileInformations->get_file_permissions(),
        fileInformations->get_file_owner(),
        fileInformations->get_file_modification_time(),
        fileInformations->get_file_creation_time());
}
