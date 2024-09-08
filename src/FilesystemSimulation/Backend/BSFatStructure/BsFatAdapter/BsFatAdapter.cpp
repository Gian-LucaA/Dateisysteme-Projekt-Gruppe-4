//
// Created by Patrick Jansen on 30.07.24.
//

#include "BsFatAdapter.h"

#include <__filesystem/path.h>

#include "../FatFolderInformation.h"
#include "../../../helpers/DateTimeHelper.h"
#include "QDebug"

void BsFatAdapter::createSystem(size_t diskSize, size_t sizeBlock) {
    bs_fat = BsFat::createBsFat(diskSize, sizeBlock);
}

bool BsFatAdapter::createFile(size_t szFile,
                              std::string path,
                              std::string fileend,
                              std::string filekind,
                              std::string permissions,
                              std::string owner) {
    return BsFile::createFile(bs_fat, szFile, path, fileend, filekind, permissions, owner, DateTimeHelper::getCurrentDateTimeMS(), DateTimeHelper::getCurrentDateTimeMS());
}

bool BsFatAdapter::createFile(size_t szFile, std::string path, std::string fileend, std::string filekind, std::string permissions, std::string owner, long long modification_time, long long creation_time)
{
    return BsFile::createFile(bs_fat, szFile, path, fileend, filekind, permissions, owner, modification_time, creation_time);
}

FileInformationInterface *BsFatAdapter::getFile(std::string path) {
    return bs_fat->get_dtf()->get_file(path)->get_file_information(bs_fat);
}

bool BsFatAdapter::removeFile(std::string path) {
    if(!bs_fat->get_dtf()->remove_file(path)){
        return false;
    }
    std::vector<std::string> pathVector = BsDtf::split(path, '/');
    return BsFile::deleteFile(bs_fat, pathVector[pathVector.size() - 1]);
}

bool BsFatAdapter::removeFolder(std::string path)
{
    std::vector<std::string> pathVector = BsDtf::split(path, '/');
    if(pathVector.size() == 0 ){
        qCritical() << "Der Root-Folder kann nicht gelöscht werden!";
        return false;
    }

    std::string removeFolderName = pathVector[pathVector.size() - 1];

    pathVector.pop_back();

    BsDtf* folderOver;
    if(pathVector.size() == 0){
        folderOver = bs_fat->get_dtf();
    } else {
        folderOver = bs_fat->get_dtf()->getFolderByPath(BsDtf::join(pathVector));
    }

    if(folderOver == nullptr){
        return false;
    }

    if(!folderOver->getFolderByName(removeFolderName)->remove_dtf(bs_fat)){
        return false;
    }
    return folderOver->removeFolderByName(removeFolderName);
}

void BsFatAdapter::defragDisk() {
    bs_fat->defragDisk(0);
}

float BsFatAdapter::getFragmentation() {
    return bs_fat->getFragmentation();
}

void BsFatAdapter::clearFileSystem() {
    delete[] bs_fat->get_files();
    delete[] bs_fat->get_block_states();
    bs_fat->get_dtf()->deleteDtf();
    delete bs_fat->get_dtf();
    delete bs_fat;
}

bool BsFatAdapter::createFolder(std::string path) {
    return bs_fat->get_dtf()->createFolder(path);
}

std::vector<std::string> BsFatAdapter::getDirectoryTree() {
    std::vector<std::string> directory_tree;
    bs_fat->get_dtf()->getDirectoryTree(&directory_tree, "/");
    return directory_tree;
}

std::map<int, std::pair<BlockState, int>> BsFatAdapter::getBlockStates() {
    return bs_fat->getBlockStates();
}

bool BsFatAdapter::setBlock(int blockIndex, BlockState blockstate)
{
    return bs_fat->set_block_state(blockIndex, blockstate);
}

FolderInformationInterface *BsFatAdapter::getFolder(std::string path) {
    BsDtf *folder = bs_fat->get_dtf()->getFolderByPath(path);
    return new FatFolderInformation(folder->get_folder_name(), folder->get_size(bs_fat));
}

size_t BsFatAdapter::getPartitionUsage()
{
    return bs_fat->get_system_usage();
}

bool BsFatAdapter::copyFileToFileSystem(std::string pathToCopy, FileInformationInterface *fileInformations, FileSystemInterface *fileSystem)
{
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
