//
// Created by Patrick Jansen on 12.08.24.
//

#ifndef FILEINFORMATIONINTERFACE_H
#define FILEINFORMATIONINTERFACE_H
#include <string>


class FileInformationInterface {
public:
    virtual ~FileInformationInterface() = default;

    virtual std::string get_file_name() = 0;

    virtual std::string get_file_kind() = 0;

    virtual std::string get_file_prefix() = 0;

    virtual std::string get_file_permissions() = 0;

    virtual std::string get_file_owner() = 0;

    virtual long long get_file_modification_time() = 0;

    virtual long long get_file_creation_time() = 0;

    virtual size_t get_file_size() = 0;
};


#endif //FILEINFORMATIONINTERFACE_H
