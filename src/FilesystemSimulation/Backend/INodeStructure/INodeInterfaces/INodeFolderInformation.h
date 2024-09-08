//
// Created by Tim Preisler on 18.08.24.
//

#ifndef INODEFOLDERINFORMATION_H
#define INODEFOLDERINFORMATION_H

#include <utility>

#include "../../InterfaceForMBR/FolderInformationInterface.h"


class INodeFolderInformation final : public FolderInformationInterface{
public:
    [[nodiscard]] INodeFolderInformation(std::string folder_name, const size_t folder_size)
        : folder_name(std::move(folder_name)),
          folder_size(folder_size) {
    }

    std::string get_folder_name() override;

    size_t get_folder_size() override;

private:
    std::string folder_name;
    size_t folder_size;
};



#endif //INODEFOLDERINFORMATION_H
