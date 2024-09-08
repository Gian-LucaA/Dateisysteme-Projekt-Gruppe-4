//
// Created by Tim Preisler on 27.06.24.
//

#ifndef BSFILE_H
#define BSFILE_H

#include "BsFat.h"
#include "BsCluster.h"
#include "FatFileInformation.h"

class BsDtfEntry;
class BsFat;
class FatFileInformation;

class BsFile {
public:

    static bool createFile(BsFat *pFat, size_t szFile, std::string filename, std::string fileend,
                              std::string filekind,
                              std::string permissions,
                              std::string owner,
                              long long modification_time,
                              long long creation_time);

    static bool deleteFile(const BsFat *pFat, std::string fileName);

    [[nodiscard]] std::string get_filename() const {
        return filename;
    }

    void set_filename(const std::string &filename) {
        this->filename = filename;
    }

    [[nodiscard]] std::string get_file_prefix() const {
        return filePrefix;
    }

    void set_file_prefix(std::string file_prefix) {
        filePrefix = file_prefix;
    }

    [[nodiscard]] std::string get_filekind() const {
        return filekind;
    }

    void set_filekind(std::string filekind) {
        this->filekind = filekind;
    }

    [[nodiscard]] BsCluster *get_bs_cluster() const {
        return bsCluster;
    }

    void set_bs_cluster(BsCluster *const bs_cluster) {
        bsCluster = bs_cluster;
    }

    size_t get_file_size(BsFat *pFat);

    FatFileInformation *get_file_information(BsFat *pFat);

    std::string get_file_permissions() {
        return file_permissions;
    }

    std::string get_file_owner() {
        return file_owner;
    }
    std::string get_path() {
        return path;
    }

    [[nodiscard]] long long get_file_modification_time() const {
        return file_modification_time;
    }

    [[nodiscard]] long long get_file_creation_time() const {
        return file_creation_time;
    }

private:
    std::string filename;
    std::string filePrefix;
    std::string filekind;
    std::string file_permissions;
    std::string file_owner;
    std::string path;
    size_t szFile;
    long long file_modification_time;
    long long file_creation_time;
    BsCluster *bsCluster;
};


#endif //BSFILE_H
