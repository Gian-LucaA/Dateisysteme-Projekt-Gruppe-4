//
// Created by Patrick Jansen on 26.07.24.
//

#ifndef BSDTF_H
#define BSDTF_H
#include <vector>

#include "BsFile.h"
#include "BsFat.h"


class BsFile;
class BsDtfEntry;
class BsFat;
class BsFatAdapter;

class BsDtf {
public:
    BsDtf(std::string folder_name);

    void insertFile(std::string path, BsFile *entry);

    bool createFolder(const std::string &path);

    void getDirectoryTree(std::vector<std::string> *vector, std::string currentPath);

    size_t get_size(BsFat *pFat);

    static std::vector<std::string> split(const std::string &str, char delimiter);

    BsFile *getFileByName(const std::string &filename);

    bool removeFileByName(const std::string &filename);

    BsDtf *getFolderByName(const std::string &foldername);

    BsDtf *getFolderByPath(std::string path);

    bool removeFolderByName(const std::string &foldername);

    static std::string join(const std::vector<std::string> &str);

    BsFile *get_file(std::string filePath);

    bool remove_file(std::string filePath);

    void remove_folder(std::string filePath);

    bool remove_dtf(BsFat* pFat);

    void deleteDtf();

    bool hasFolder(std::string folder_name);

    BsDtf *getFolder(std::string folder_name);

    std::string get_folder_name() { return folder_name; }

private:
    std::vector<BsFile *> files;
    std::vector<BsDtf *> folder;

    std::string folder_name;
};


#endif //BSDTF_H
