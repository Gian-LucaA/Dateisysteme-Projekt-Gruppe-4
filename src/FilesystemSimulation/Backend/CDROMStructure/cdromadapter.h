#ifndef CDROMADAPTER_H
#define CDROMADAPTER_H

#include "../BSFatStructure/BsFatAdapter/BsFatAdapter.h"

class CDROMAdapter final : public BsFatAdapter
{
public:
    CDROMAdapter();

    bool removeFile(std::string path) override;

    bool removeFolder(std::string path) override;

    void defragDisk() override;
private:

};

#endif // CDROMADAPTER_H
