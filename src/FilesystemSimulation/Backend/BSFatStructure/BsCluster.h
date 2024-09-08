//
// Created by Tim Preisler on 27.06.24.
//

#ifndef BSCLUSTER1_H
#define BSCLUSTER1_H

#include <iostream>


class BsCluster {
public:
    static BsCluster appendBsCluster(BsCluster *, size_t);

    [[nodiscard]] int get_index() const {
        return index;
    }

    void set_index(const int index) {
        this->index = index;
    }

    [[nodiscard]] BsCluster *get_next() const {
        return next;
    }

    void set_next(BsCluster *const next) {
        this->next = next;
    }

    [[nodiscard]] BsCluster *get_prev() const {
        return prev;
    }

    void set_prev(BsCluster *const prev) {
        this->prev = prev;
    }

private:
    int index;
    BsCluster *next;
    BsCluster *prev;
};


#endif //BSCLUSTER1_H
