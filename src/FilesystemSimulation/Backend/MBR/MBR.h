//
// Created by Patrick Jansen on 31.07.24.
//

#ifndef MBR_H
#define MBR_H
#include "../BSFatStructure/BsFatAdapter/BsFatAdapter.h"
#include "../INodeStructure/INodeInterfaces/INodeAdapter.h"
#include "../InterfaceForMBR/FileSystemInterface.h"
#include "../CDROMStructure/cdromadapter.h"


class MBR {
public:
    MBR();

    void set_system(int partition);

    FileSystemInterface *get_system();

    FileSystemInterface *get_system(int partition);

private:
    FileSystemInterface *active_system;
    FileSystemInterface *partition_table[3] = {new BsFatAdapter(), new INodeAdapter(), new CDROMAdapter()};
};


#endif //MBR_H
