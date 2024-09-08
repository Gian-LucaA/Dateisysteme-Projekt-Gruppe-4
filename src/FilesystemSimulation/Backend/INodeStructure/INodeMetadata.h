//
// Created by Tim Preisler on 31.07.24.
//

#ifndef INODEMETADATA_H
#define INODEMETADATA_H
#include <string>

#include "INodeType.h"

using namespace::std;

class INodeMetadata {
public:
    INodeType type;
    std::string owner;
    string permissions;
    long long creationTime;
    long long modificationTime;
    size_t size;
    string fileKind;
    string fileEnd;

//public:

    INodeMetadata(INodeType t, std::string o, string p, const int size, long long modificationTime, long long creationTime) {
        this->type = t;
        this->owner = o;
        this->permissions = p;
        this->modificationTime = modificationTime;
        this->creationTime = creationTime;
        this->size = size;
    }

    INodeMetadata(INodeType t, string fileKind, string fileEnd, std::string o, string p, const int size, long long modificationTime, long long creationTime) {
        this->type = t;
        this->owner = o;
        this->permissions = p;
        this->modificationTime = modificationTime;
        this->creationTime = creationTime;
        this->size = size;
        this->fileEnd = fileEnd;
        this->fileKind = fileKind;
    }

    [[nodiscard]] INodeType get_type() const {
        return type;
    }

    void set_type(const INodeType type) {
        this->type = type;
    }

    [[nodiscard]] std::string get_owner() const {
        return owner;
    }

    void set_owner(const std::string &owner) {
        this->owner = owner;
    }

    [[nodiscard]] string get_permissions() const {
        return permissions;
    }

    void set_permissions(const string permissions) {
        this->permissions = permissions;
    }

    [[nodiscard]] long long get_creation_time() const {
        return creationTime;
    }

    void set_creation_time(const long long creation_time) {
        creationTime = creation_time;
    }

    [[nodiscard]] long long get_modification_time() const {
        return modificationTime;
    }

    void set_modification_time(const long long modification_time) {
        modificationTime = modification_time;
    }

    [[nodiscard]] size_t get_size() const {
        return size;
    }

    void set_size(const size_t size) {
        this->size = size;
    }

    [[nodiscard]] string get_fileKind() const {
        return fileKind;
    }

    void set_fileKind(const string fileKind) {
        this->fileKind = fileKind;
    }

    [[nodiscard]] string get_fileEnd() const {
        return fileEnd;
    }

    void set_fileEnd(const string fileEnd) {
        this->fileEnd = fileEnd;
    }
};

#endif //INODEMETADATA_H
