//
// Created by Tim Preisler on 27.06.24.
//

#include "BsCluster.h"

#include <qDebug>

BsCluster BsCluster::appendBsCluster(BsCluster *list, size_t blockIndex) {
    if (list != nullptr) {
        BsCluster *tail = list;
        while (tail->next != nullptr) {
            tail = tail->next;
        }

        auto *newEntry = new BsCluster();
        if (newEntry == nullptr) {
            qCritical() << "Dies ist eine Debug-Nachricht.";
        } else {
            newEntry->index = blockIndex;
            newEntry->prev = tail;
            newEntry->next = nullptr;

            tail->next = newEntry;

            return *newEntry;
        }

    }
}
