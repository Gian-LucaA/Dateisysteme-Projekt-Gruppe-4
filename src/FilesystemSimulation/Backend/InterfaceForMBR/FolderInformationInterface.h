//
// Created by Patrick Jansen on 10.08.24.
//

#ifndef FOLDERINFORMATIONINTERFACE_H
#define FOLDERINFORMATIONINTERFACE_H
#include <string>


class FolderInformationInterface {
public:
    virtual ~FolderInformationInterface() = default;

    virtual std::string get_folder_name() = 0;

    virtual size_t get_folder_size() = 0;
};


#endif //FOLDERINFORMATIONINTERFACE_H
