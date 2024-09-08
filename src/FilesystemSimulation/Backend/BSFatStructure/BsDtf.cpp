//
// Created by Patrick Jansen on 26.07.24.
//

#include "BsDtf.h"

#include <sstream>
#include <__filesystem/path.h>

#include "BsFile.h"

#include "qDebug.h"

BsDtf::BsDtf(std::string folder_name) {
    this->folder_name = folder_name;
}

void BsDtf::insertFile(std::string path, BsFile *entry) {
    std::vector<std::string> pathVector = split(path, '/');
    if (pathVector.size() == 1) {
        files.push_back(entry);
    } else {
        std::string dtf_name = pathVector[0];
        for (int i = 0; i < folder.size(); i++) {
            if (!folder[i]->folder_name.compare(dtf_name)) {
                pathVector.erase(pathVector.begin());
                folder[i]->insertFile(join(pathVector), entry);
                return;
            }
        }
        pathVector.erase(pathVector.begin());
        BsDtf *insertBsDtf = new BsDtf(dtf_name);
        folder.push_back(insertBsDtf);
        insertBsDtf->insertFile(join(pathVector), entry);
    }
}

std::vector<std::string> BsDtf::split(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    if(tokens[0] == ""){
        tokens.erase(tokens.begin());
    }
    return tokens;
}

BsFile *BsDtf::getFileByName(const std::string &filename) {
    for (int i = 0; i < files.size(); i++) {
        if (!files[i]->get_filename().compare(filename)) {
            return files[i];
        }
    }
    qCritical() << "File not found";
}

bool BsDtf::removeFileByName(const std::string &filename) {
    for (int i = 0; i < files.size(); i++) {
        if (!files[i]->get_filename().compare(filename)) {
            files.erase(files.begin() + i);
            return true;
        }
    }
    qCritical() << "File not found";
    return false;
}

BsDtf *BsDtf::getFolderByName(const std::string &foldername) {
    for (int i = 0; i < folder.size(); i++) {
        if (!folder[i]->folder_name.compare(foldername)) {
            return folder[i];
        }
    }
    qCritical() << "Folder not found";
}

BsDtf *BsDtf::getFolderByPath(std::string path) {
    std::vector<std::string> pathVector = split(path, '/');

    if (hasFolder(pathVector[0])) {
        if (pathVector.size() == 1) {
            return getFolderByName(pathVector[0]);
        }
        std::string folder_name_string = pathVector[0];
        pathVector.erase(pathVector.begin());
        return getFolderByName(folder_name_string)->getFolderByPath(join(pathVector));
    }

    qCritical() << "Invalid folder name!";
}

bool BsDtf::removeFolderByName(const std::string &foldername) {
    for (int i = 0; i < folder.size(); i++) {
        if (!folder[i]->folder_name.compare(foldername)) {
            if (folder[i]->files.size() > 0) {
                throw std::invalid_argument("Folder not empty");
            }
            folder.erase(folder.begin() + i);
            return true;
        }
    }
    qCritical() << "Folder not found";
    return false;
}

std::string BsDtf::join(const std::vector<std::string> &str) {
    std::ostringstream oss;
    char delimiter = '/';
    for (size_t i = 0; i < str.size(); i++) {
        if (i != 0) {
            oss << delimiter;
        }
        oss << str[i];
    }
    return oss.str();
}

BsFile *BsDtf::get_file(std::string filePath) {
    std::vector<std::string> pathVector = BsDtf::split(filePath, '/');
    if (pathVector.size() == 1) {
        return this->getFileByName(pathVector[0]);
    } else {
        std::string folder_name = pathVector[0];
        pathVector.erase(pathVector.begin());
        return this->getFolderByName(folder_name)->get_file(join(pathVector));
    }
}

bool BsDtf::remove_file(std::string filePath) {
    std::vector<std::string> pathVector = BsDtf::split(filePath, '/');
    if (pathVector.size() == 1) {
        return this->removeFileByName(pathVector[0]);
    } else {
        std::string folder_name = pathVector[0];
        pathVector.erase(pathVector.begin());
        return this->getFolderByName(folder_name)->remove_file(join(pathVector));
    }
}

void BsDtf::remove_folder(std::string filePath) {
    std::vector<std::string> pathVector = BsDtf::split(filePath, '/');
    if (pathVector.size() == 1) {
        this->removeFolderByName(pathVector[0]);
    } else {
        std::string folder_name = pathVector[0];
        pathVector.erase(pathVector.begin());
        this->getFolderByName(folder_name)->get_file(join(pathVector));
    }
}

bool BsDtf::remove_dtf(BsFat* pFat)
{
    while(!files.empty()){
        BsFile* file = files[0];
        if(!pFat->get_dtf()->remove_file(file->get_path())){
            return false;
        }
        std::vector<std::string> pathVector = BsDtf::split(file->get_path(), '/');
        if(!BsFile::deleteFile(pFat, pathVector[pathVector.size() - 1])){
            return false;
        }
    }
    for(BsDtf* folder : folder){
        if(!folder->remove_dtf(pFat)){
            return false;
        }
    }
    return true;
}

void BsDtf::deleteDtf() {
    files.clear();
    for (BsDtf *bs_dtf: folder) {
        bs_dtf->deleteDtf();
    }
    folder.clear();
}

bool BsDtf::hasFolder(std::string folder_name) {
    for (BsDtf *folder: folder) {
        if (!folder->get_folder_name().compare(folder_name)) {
            return true;
        }
    }

    return false;
}

BsDtf *BsDtf::getFolder(std::string folder_name) {
    for (BsDtf *folder: folder) {
        if (!folder->get_folder_name().compare(folder_name)) {
            return folder;
        }
    }

    qCritical() << "Invalid folder name";
}

bool BsDtf::createFolder(const std::string &path) {
    std::vector<std::string> pathVector = split(path, '/');

    if (pathVector.size() == 1) {
        if (hasFolder(pathVector[0])) {
            qCritical() << "Folder already created!";
            return false;
        }
        folder.push_back(new BsDtf(pathVector[0]));
    } else {
        if (hasFolder(pathVector[0])) {
            BsDtf *insertIntoFolder = getFolder(pathVector[0]);
            if(insertIntoFolder == nullptr){
                return false;
            }
            pathVector.erase(pathVector.begin());
            if(!insertIntoFolder->createFolder(join(pathVector))){
                return false;
            }
        } else {
            folder.push_back(new BsDtf(pathVector[0]));
            BsDtf *insertIntoFolder = getFolder(pathVector[0]);
            if(insertIntoFolder == nullptr){
                return false;
            }
            pathVector.erase(pathVector.begin());
            if(!insertIntoFolder->createFolder(join(pathVector))){
                return false;
            }
        }
    }
    return true;
}

void BsDtf::getDirectoryTree(std::vector<std::string> *vector, std::string currentPath) {
    vector->push_back(currentPath + folder_name + "/");
    for (BsFile *file: files) {
        vector->push_back(currentPath + folder_name + "/" + file->get_filename());
    }
    for (BsDtf *folder: folder) {
        folder->getDirectoryTree(vector, currentPath + folder_name + "/");
    }
}

size_t BsDtf::get_size(BsFat *pFat) {
    size_t size = 0;
    for (BsFile *file: files) {
        size += file->get_file_size(pFat);
    }

    for (BsDtf *folder: folder) {
        size += folder->get_size(pFat);
    }

    return size;
}
