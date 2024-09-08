//
// Created by Patrick Jansen on 12.08.24.
//

#include "FatFileInformation.h"

std::string FatFileInformation::get_file_name() {
    return file_name;
}

std::string FatFileInformation::get_file_kind() {
    return file_kind;
}

std::string FatFileInformation::get_file_prefix() {
    return file_prefix;
}

std::string FatFileInformation::get_file_permissions() {
    return file_permissions;
}

std::string FatFileInformation::get_file_owner() {
    return file_owner;
}

long long FatFileInformation::get_file_modification_time() {
    return file_modification_time;
}

long long FatFileInformation::get_file_creation_time() {
    return file_creation_time;
}

size_t FatFileInformation::get_file_size() {
    return file_size;
}
