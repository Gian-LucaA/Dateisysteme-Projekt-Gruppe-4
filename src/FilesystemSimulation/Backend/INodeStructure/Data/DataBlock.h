//
// Created by Tim Preisler on 31.07.24.
//

#ifndef DATABLOCK_H
#define DATABLOCK_H
#include <vector>
#include "../../BSFatStructure/BlockState.h"

using namespace std;

class DataBlock {
public:
    DataBlock(){};

    DataBlock(BlockState block_state) : block_state_(block_state) {
    };

    void setData(std::vector<char> vector);

    void clearData();

    [[nodiscard]] std::vector<char> get_vector() const {
        return vector;
    }

    void set_vector(const std::vector<char> &vector) {
        this->vector = vector;
    }
    
    [[nodiscard]] BlockState get_block_state() const {
        return block_state_;
    }

    void set_block_state(const BlockState block_state_) {
        this->block_state_ = block_state_;
    }

private:
    BlockState block_state_;
    std::vector<char> vector;
    //std::vector<DataBlock*> blocks_;
};


#endif //DATABLOCK_H
