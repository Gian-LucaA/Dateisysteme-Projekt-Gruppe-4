//
// Created by Patrick Jansen on 25.07.24.
//

#include "FileAllocationTableRow.h"
#include "qDebug.h"

FileAllocationTableRow::FileAllocationTableRow() {
    blockState = FREE;
}

BlockState FileAllocationTableRow::getBlockState() {
    return blockState;
}

BsCluster FileAllocationTableRow::getBsCluster() {
    if (blockState == OCCUPIED) {
        return bsCluster;
    } else {
        // throw std::invalid_argument("No Cluster available!");
        qCritical() << "Fehler: Kein Cluster vefügbar.";
    }
}

void FileAllocationTableRow::setBlockState(BlockState block_state) {
    if (block_state == OCCUPIED) {
        // throw std::invalid_argument("OCCUPIED is not valid here");
        qWarning() << "Fehler: OCCUPIED ist hier nicht erlaubt.";
    } else {
        blockState = block_state;
    }
}

void FileAllocationTableRow::setBsCluster(BsCluster bs_cluster) {
    bsCluster = bs_cluster;
    blockState = OCCUPIED;
}

void FileAllocationTableRow::cleanRow() {
    blockState = FREE;
}
