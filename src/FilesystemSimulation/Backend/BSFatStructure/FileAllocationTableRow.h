//
// Created by Patrick Jansen on 25.07.24.
//

#ifndef FILEALLOCATIONTABLEROW_H
#define FILEALLOCATIONTABLEROW_H
#include <__fwd/string.h>
#include "BlockState.h"
#include "BsCluster.h"


class FileAllocationTableRow {
public:
    FileAllocationTableRow();

    BlockState getBlockState();

    BsCluster getBsCluster();

    void setBlockState(BlockState block_state);

    void setBsCluster(BsCluster bs_cluster);

    void cleanRow();

private:
    BlockState blockState;
    BsCluster bsCluster;
};


#endif //FILEALLOCATIONTABLEROW_H
