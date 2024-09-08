//
// Created by Tim Preisler on 27.06.24.
//

#ifndef BSFAT_H
#define BSFAT_H


#include <iostream>
#include <map>
#include <vector>

#include "FileAllocationTableRow.h"
#include "BlockState.h"

#include "BsDtf.h"

class BsDtf;
class BsFile;

class BsFat {
public:
    static BsFat *createBsFat(size_t, size_t);

    size_t getFreeDiskSpace();

    void freeBsFat();

    float getFragmentation();


    int block_is_occupied(size_t i);

    void defragDisk(int callCount);


    [[nodiscard]] BsFat() = default;

    [[nodiscard]] BsFat(const size_t disk_size, const size_t block_size, const size_t block_count,
                        FileAllocationTableRow *const fileAllocationTable, BsFile *const files)
        : diskSize(disk_size),
          blockSize(block_size),
          blockCount(block_count),
          fileAllocationTable(fileAllocationTable),
          files(files) {
    }

    [[nodiscard]] size_t get_disk_size() const {
        return diskSize;
    }

    void set_disk_size(const size_t disk_size) {
        diskSize = disk_size;
    }

    [[nodiscard]] size_t get_block_size() const {
        return blockSize;
    }

    void set_block_size(const size_t block_size) {
        blockSize = block_size;
    }

    [[nodiscard]] size_t get_block_count() const {
        return blockCount;
    }

    void set_block_count(const size_t block_count) {
        blockCount = block_count;
    }

    [[nodiscard]] FileAllocationTableRow *get_block_states() const {
        return fileAllocationTable;
    }

    void set_block_states(FileAllocationTableRow *const fileAllocationTable) {
        BsFat::fileAllocationTable = fileAllocationTable;
    }

    bool set_block_state(int index, BlockState blockstate);

    [[nodiscard]] BsFile *get_files() const {
        return files;
    }

    void set_files(BsFile *const files) {
        this->files = files;
    }

    BsDtf *get_dtf() {
        return directoryTableFormat;
    }

    size_t get_system_usage();

    std::map<int, std::pair<BlockState, int>> getBlockStates();

private:
    size_t diskSize; // Größe der gesamten Platte
    size_t blockSize; // Größe eines Blocks
    size_t blockCount; // Anzahl der Blöcke auf der Platte
    FileAllocationTableRow *fileAllocationTable; // Array zur Speicherung des Status jedes Blocks
    BsFile *files;
    BsDtf *directoryTableFormat;
};


#endif //BSFAT_H
