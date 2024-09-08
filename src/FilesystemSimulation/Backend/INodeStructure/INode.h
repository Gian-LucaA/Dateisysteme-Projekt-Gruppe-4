#ifndef INODE_H
#define INODE_H

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

#include "INodeType.h"
#include "INodeMetadata.h"
#include "Data/DoubleIndirect.h"
#include "Data/SingleIndirect.h"
#include "Data/TripleIndirect.h"
#include "../../helpers/DateTimeHelper.h"

using namespace::std;

class INode {
    int index{};

public:
    INodeMetadata metadata;
    int* data = new int[12];
    Singleindirect *singleindirect{};
    DoubleIndirect *double_indirect{};
    TripleIndirect *triple_indirect{};
    //Zur Speicherung der Orndereinträge.
    unordered_map<std::string, int> directoryEntries;

    INode(const int index, INodeType type) :
        INode(index, type, "", "", "", "", 0, DateTimeHelper::getCurrentDateTimeMS(), DateTimeHelper::getCurrentDateTimeMS()){
    };

    INode(const int index, INodeType type, string fileEnd, string fileKind, string owner, string permission,
          size_t size, long long modificationTime, long long creationTime) :
        index(index),
        metadata(type, fileKind, fileEnd, std::move(owner), permission, size, modificationTime, creationTime) {
        //Initialisierung aller Feldeinträge auf -1
        for(int i = 0; i < 12; i++){
            data[i] = -1;
        }
    };

    /**
     * @brief getAllUsedBlockNumbers Ermittelt alle genutzen Datenblocknummern zur Speicherung der Daten.
     * @return Eine Liste von genutzen Nummern.
     */
    vector<int> getAllUsedBlockNumbers();

    /**
     * @brief isFile Ermittelt, ob es ich um eine Datei handelt.
     * @return Bei Erfolg true, andernfalls false.
     */
    bool isFile();

    [[nodiscard]] INodeMetadata get_metadata() const {
        return metadata;
    }

    [[nodiscard]] int get_index() const {
        return index;
    }
};

#endif // INODE_H
