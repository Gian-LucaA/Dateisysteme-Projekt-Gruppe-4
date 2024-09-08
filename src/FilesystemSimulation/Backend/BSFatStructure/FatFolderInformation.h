//
// Created by Patrick Jansen on 10.08.24.
//

#ifndef FATFOLDERINFORMATION_H
#define FATFOLDERINFORMATION_H
#include <utility>

#include "../InterfaceForMBR/FolderInformationInterface.h"


class FatFolderInformation final : public FolderInformationInterface {
public:
    FatFolderInformation(std::string folder_name, size_t folder_size)
        : folder_name(std::move(folder_name)),
          folder_size(folder_size) {
    }

    std::string get_folder_name() override;

    size_t get_folder_size() override;

private:
    std::string folder_name;
    size_t folder_size;
};


#endif //FATFOLDERINFORMATION_H
