//
// Created by Patrick Jansen on 30.07.24.
//

#ifndef FILESYSTEMINTERFACE_H
#define FILESYSTEMINTERFACE_H
#include <map>
#include <string>

#include "FolderInformationInterface.h"
#include "../BSFatStructure/BsFile.h"


class FileSystemInterface {
public:
    virtual ~FileSystemInterface() = default;

    virtual void createSystem(size_t diskSize, size_t sizeBlock) = 0;

    virtual bool createFile(size_t szFile,
                            std::string path,
                            std::string fileend,
                            std::string filekind,
                            std::string permissions,
                            std::string owner) = 0;

    virtual bool createFile(size_t szFile,
                            std::string path,
                            std::string fileend,
                            std::string filekind,
                            std::string permissions,
                            std::string owner,
                            long long modification_time,
                            long long creation_time) = 0;

    virtual bool createFolder(std::string path) = 0;

    virtual FileInformationInterface *getFile(std::string path) = 0;

    virtual FolderInformationInterface *getFolder(std::string path) = 0;

    virtual bool removeFile(std::string path) = 0;

    virtual bool removeFolder(std::string path) = 0;

    virtual void defragDisk() = 0;

    virtual float getFragmentation() = 0;

    virtual std::vector<std::string> getDirectoryTree() = 0; //

    virtual void clearFileSystem() = 0; //alle files löschen auch root

    virtual std::map<int, std::pair<BlockState, int>> getBlockStates() = 0;

    virtual bool setBlock(int blockIndex, BlockState blockstate) = 0;

    virtual size_t getPartitionUsage() = 0;

    virtual bool copyFileToFileSystem(std::string pathToCopy,
                                      FileInformationInterface* fileInformations,
                                      FileSystemInterface* fileSystem) = 0;
};


#endif //FILESYSTEMINTERFACE_H
