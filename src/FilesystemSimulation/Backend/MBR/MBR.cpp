//
// Created by Patrick Jansen on 31.07.24.
//

#include "MBR.h"


MBR::MBR() {

}

void MBR::set_system(int partition)
{
    active_system = partition_table[partition];
}

FileSystemInterface *MBR::get_system() {
    return active_system;
}

FileSystemInterface *MBR::get_system(int partition)
{
    return partition_table[partition];
}
