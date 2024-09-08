#include "DataHandler.h"
#include "QDebug"
#include "DataBlock.h"

DataHandler::DataHandler() {
}

size_t DataHandler::fillVector(std::vector<int> *data_blocks, size_t blocks_left, INodeSystem *inode_system) {
    //Führe das Füllen solange aus, wie die übrige Blockzahl > 0 und die Größe der Liste
    //der Datenblock-Pointer nicht der Größe der Blockgröße/Größe eines Pointers
    //überschreitet.
    while (blocks_left > 0 && data_blocks->size() < inode_system->blockSize / sizeof(int)) {
        int random = getRandomNumber(inode_system);

        data_blocks->push_back(random);
        inode_system->data_blocks[random] = DataBlock(BlockState::OCCUPIED);
        blocks_left--;
    }
    return blocks_left;
}

size_t DataHandler::fillVector(std::vector<Singleindirect> singleindirects, size_t blocks_left,
                                INodeSystem *inode_system) {
    //Führe das Füllen solange aus, wie die übrige Blockzahl > 0 und die Größe der Liste
    //der Einfach-Indirekten Pointer nicht der Größe der Blockgröße/Größe eines Pointers
    //überschreitet.
    while (blocks_left > 0 && singleindirects.size() < inode_system->blockSize / sizeof(int)) {
        Singleindirect singleindirect;
        blocks_left = fillVector(&singleindirect.dataBlocks, blocks_left, inode_system);
        singleindirects.push_back(singleindirect);
    }
    return blocks_left;
}

size_t DataHandler::fillVector(std::vector<DoubleIndirect> double_indirects, size_t blocks_left,
                                INodeSystem *inode_system) {
    //Führe das Füllen solange aus, wie die übrige Blockzahl > 0 und die Größe der Liste
    //der Zweifach-Indirekten Pointer nicht der Größe der Blockgröße/Größe eines Pointers
    //überschreitet.
    while (blocks_left > 0 && double_indirects.size() < inode_system->blockSize / sizeof(int)) {
        DoubleIndirect double_indirect;
        blocks_left = fillVector(double_indirect.singleindirects, blocks_left, inode_system);
        double_indirects.push_back(double_indirect);
    }
    return blocks_left;
}

void DataHandler::fillDataBlocks(INode *inode, INodeSystem *inode_system) {
    const size_t countBlocks = (inode->metadata.get_size() + inode_system->blockSize - 1) / inode_system->blockSize;
    size_t blocksLeft = countBlocks;
    for (int i = 0; i < 12 && blocksLeft > 0; i++) {
        int random = getRandomNumber(inode_system);
        if(random == -1){
            qCritical() << "Fehler bei der Blockspeicherung.";
        }
        DataBlock data_block;
        data_block.set_block_state(BlockState::OCCUPIED);
        inode_system->data_blocks[random] = data_block;
        inode->data[i] = random;

        blocksLeft--;
    }

    if (blocksLeft > 0) {
        inode->singleindirect = new Singleindirect;
        blocksLeft = fillVector(&inode->singleindirect->dataBlocks, blocksLeft, inode_system);
    }

    if (blocksLeft > 0) {
        inode->double_indirect = new DoubleIndirect;
        blocksLeft = fillVector(inode->double_indirect->singleindirects, blocksLeft, inode_system);
    }

    if (blocksLeft > 0) {
        inode->triple_indirect = new TripleIndirect;
        fillVector(inode->triple_indirect->double_indirects, blocksLeft, inode_system);
    }
}

void DataHandler::setDataBlockIndex(INode *node, int block_index, int new_data_index, INodeSystem *inode_system){
    if(block_index > node->metadata.size / inode_system->blockSize){
        qWarning() << "Der Index exisitert nicht!";
        return;
    }
    int counted_index = 0;

    for(int i = 0; i < 12; i++){
        if(block_index == counted_index){
            node->data[i] = new_data_index;
            return;
        }
        counted_index++;
    }

    if(node->singleindirect != nullptr){
        for(int i = 0; i < node->singleindirect->dataBlocks.size(); i++){
            if(block_index == counted_index){
                node->singleindirect->dataBlocks.at(i) = new_data_index;
                return;
            }
            counted_index++;
        }
    }

    if(node->double_indirect != nullptr){
        for(Singleindirect s : node->double_indirect->singleindirects){
            for(int i = 0; i < s.dataBlocks.size(); i++){
                if(block_index == counted_index){
                    s.dataBlocks.at(i) = new_data_index;
                    return;
                }
                counted_index++;
            }
        }
    }

    if(node->triple_indirect != nullptr){
        for(DoubleIndirect &d : node->triple_indirect->double_indirects){
            for(Singleindirect s : d.singleindirects){
                for(int i = 0; i < s.dataBlocks.size(); i++){
                    if(block_index == counted_index){
                        s.dataBlocks.at(i) = new_data_index;
                        return;
                    }
                    counted_index++;
                }
            }
        }
    }
}

int DataHandler::getRandomNumber(INodeSystem* inode_system){
    int random;
    int noPlaceFound = 0;
    do {
        random = arc4random() % inode_system->blockCount;
        noPlaceFound++;
    } while (inode_system->data_blocks[random].get_block_state() != BlockState::FREE && noPlaceFound < 10);

    //wenn nach 10-maligen Suchen einer Zufallszahl kein Treffer erziehlt wurde, wird die erste freie Stelle
    //im Blockspeicher genommen, um lange Wartezeiten zu verhinden.
    if(noPlaceFound >= 10){
        random = -1;
        for (int var = 0; var < inode_system->blockCount; ++var) {
            if(inode_system->data_blocks[var].get_block_state() == BlockState::FREE){
                random = var;
                break;
            }
        }
    }
    return random;
}
