//
// Created by Tim Preisler on 31.07.24.
//

#include "DataBlock.h"

#include <vector>

using namespace::std;

void DataBlock::clearData(){
    this->vector.clear();
    this->block_state_= BlockState::FREE;
}
