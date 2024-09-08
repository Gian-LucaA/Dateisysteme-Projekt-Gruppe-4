#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <vector>
#include <string>

#include "../../InterfaceForMBR/FileSystemInterface.h"
#include "../INodeSystem.h"

using namespace::std;


class INodeAdapter : public FileSystemInterface {
private:
    INodeSystem *inode_system;

public:

    void createSystem(size_t diskSize, size_t sizeBlock) override;

    bool createFile(size_t szFile, std::string path, std::string fileend, std::string filekind, std::string permissions,
                    std::string owner) override;

    bool createFile(size_t szFile,
                    std::string path,
                    std::string fileend,
                    std::string filekind,
                    std::string permissions,
                    std::string owner,
                    long long modification_time,
                    long long creation_time) override;

    bool createFolder(std::string path) override;

    FileInformationInterface *getFile(std::string path) override;

    FolderInformationInterface *getFolder(std::string path) override;

    bool removeFile(std::string path) override;

    bool removeFolder(std::string path) override;

    void defragDisk() override;

    float getFragmentation() override;

    std::vector<std::string> getDirectoryTree() override;

    void clearFileSystem() override;

    //std::vector<BlockState> getBlockStates() override;

    std::map<int, std::pair<BlockState, int> > getBlockStates() override;

    bool setBlock(int blockIndex, BlockState blockstate) override;

    size_t getPartitionUsage() override;

    bool copyFileToFileSystem(std::string pathToCopy, FileInformationInterface *fileInformations,
                              FileSystemInterface *fileSystem) override;
};

#endif // FILESYSTEM_H
