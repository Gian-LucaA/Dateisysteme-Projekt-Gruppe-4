//
// Created by Tim Preisler on 18.08.24.
//

#include "../INodeInterfaces/INodeFileInformation.h"

std::string INodeFileInformation::get_file_name() {
    return this->file_name;
}

std::string INodeFileInformation::get_file_kind() {
    return this->file_kind;
}

std::string INodeFileInformation::get_file_prefix() {
    return this->file_prefix;
}

std::string INodeFileInformation::get_file_permissions() {
    return this->file_permissions;
}

std::string INodeFileInformation::get_file_owner() {
    return this->file_owner;
}

long long INodeFileInformation::get_file_modification_time() {
    return this->file_modification_time;
}

long long INodeFileInformation::get_file_creation_time() {
    return this->file_creation_time;
}

size_t INodeFileInformation::get_file_size() {
    return this->file_size;
}