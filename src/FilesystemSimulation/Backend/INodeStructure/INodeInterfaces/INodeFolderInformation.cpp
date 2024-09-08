//
// Created by Tim Preisler on 18.08.24.
//

#include "../INodeInterfaces/INodeFolderInformation.h"

#include <string>

std::string INodeFolderInformation::get_folder_name() {
    return folder_name;
}

size_t INodeFolderInformation::get_folder_size() {
    return folder_size;
}