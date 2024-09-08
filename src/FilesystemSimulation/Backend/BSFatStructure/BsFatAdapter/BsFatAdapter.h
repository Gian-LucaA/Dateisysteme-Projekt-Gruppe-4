//
// Created by Patrick Jansen on 30.07.24.
//

#ifndef BSFATADAPTER_H
#define BSFATADAPTER_H
#include "../../InterfaceForMBR/FileSystemInterface.h"


class BsFatAdapter : public FileSystemInterface {
public:
    void createSystem(size_t diskSize, size_t sizeBlock) override;

    bool createFile(size_t szFile,
                    std::string path,
                    std::string fileend,
                    std::string filekind,
                    std::string permissions,
                    std::string owner) override;

    bool createFile(size_t szFile,
                            std::string path,
                            std::string fileend,
                            std::string filekind,
                            std::string permissions,
                            std::string owner,
                            long long modification_time,
                            long long creation_time) override;

    FileInformationInterface *getFile(std::string path) override;

    bool removeFile(std::string path) override;

    bool removeFolder(std::string path) override;

    void defragDisk() override;

    float getFragmentation() override;

    void clearFileSystem() override;

    bool createFolder(std::string path) override;

    std::vector<std::string> getDirectoryTree() override;

    FolderInformationInterface *getFolder(std::string path) override;

    std::map<int, std::pair<BlockState, int>> getBlockStates() override;

    bool setBlock(int blockIndex, BlockState blockstate) override;

    size_t getPartitionUsage() override;

    bool copyFileToFileSystem(std::string pathToCopy, FileInformationInterface* fileInformations, FileSystemInterface* fileSystem) override;

private:
    BsFat *bs_fat;
};


#endif //BSFATADAPTER_H
