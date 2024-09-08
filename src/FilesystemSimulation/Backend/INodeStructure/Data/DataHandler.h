#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <vector>
#include "../INode.h"
#include "../INodeSystem.h"

using namespace::std;

class DataHandler {
public:
    DataHandler();

    static size_t fillVector(std::vector<int> *data_blocks, size_t blocks_left, INodeSystem *inode_system);

    static size_t fillVector(std::vector<Singleindirect> data_blocks, size_t blocks_left, INodeSystem *inode_system);

    static size_t fillVector(std::vector<DoubleIndirect> double_indirects, size_t blocks_left,
                              INodeSystem *inode_system);

    static void fillDataBlocks(INode *inode, INodeSystem *inode_system);

    static void setDataBlockIndex(INode *node, int block_index, int new_data_index, INodeSystem *inode_systen);

private:
    static int getRandomNumber(INodeSystem* inode_system);
};

#endif // DATAHANDLER_H
