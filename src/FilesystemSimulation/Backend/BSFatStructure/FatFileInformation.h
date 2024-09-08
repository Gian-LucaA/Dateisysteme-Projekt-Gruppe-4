//
// Created by Patrick Jansen on 12.08.24.
//

#ifndef FATFILEINFORMATION_H
#define FATFILEINFORMATION_H
#include "../InterfaceForMBR/FileInformationInterface.h"


class FatFileInformation final : public FileInformationInterface {
public:
    FatFileInformation(std::string file_name, std::string file_kind, std::string file_prefix,
                       std::string file_permissions, std::string file_owner,
                       long long file_modification_time, long long file_creation_time, size_t file_size)
        : file_name(std::move(file_name)),
          file_kind(std::move(file_kind)),
          file_prefix(std::move(file_prefix)),
          file_permissions(std::move(file_permissions)),
          file_owner(std::move(file_owner)),
          file_modification_time(file_modification_time),
          file_creation_time(file_creation_time), file_size(file_size) {
    }

    std::string get_file_name() override;

    std::string get_file_kind() override;

    std::string get_file_prefix() override;

    std::string get_file_permissions() override;

    std::string get_file_owner() override;

    long long get_file_modification_time() override;

    long long get_file_creation_time() override;

    size_t get_file_size() override;

private:
    std::string file_name;
    std::string file_kind;
    std::string file_prefix;
    std::string file_permissions;
    std::string file_owner;
    long long file_modification_time;
    long long file_creation_time;
    size_t file_size;
};


#endif //FATFILEINFORMATION_H
