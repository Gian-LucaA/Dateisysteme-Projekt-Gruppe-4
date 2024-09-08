#include "INode.h"
#include "QDebug"

vector<int> INode::getAllUsedBlockNumbers() {
    vector<int> dataBlockNumbers;

    for (int i = 0; i < 12; i++) {
        int cur_data = this->data[i];
        if (cur_data == -1) {
            return dataBlockNumbers;
        }
        dataBlockNumbers.push_back(cur_data);
    }

    if (this->singleindirect != nullptr) {
        for (int i: this->singleindirect->dataBlocks) {
            dataBlockNumbers.push_back(i);
        }
    }

    if (this->double_indirect != nullptr) {
        for (Singleindirect &s: this->double_indirect->singleindirects) {
            for (int i: s.dataBlocks) {
                dataBlockNumbers.push_back(i);
            }
        }
    }

    if (this->triple_indirect != nullptr) {
        for (DoubleIndirect &d: this->triple_indirect->double_indirects) {
            for (Singleindirect &s: d.singleindirects) {
                for (int &i: s.dataBlocks) {
                    dataBlockNumbers.push_back(i);
                }
            }
        }
    }

    return dataBlockNumbers;
}

bool INode::isFile(){
    return this->metadata.type == INodeType::FILE;
}
