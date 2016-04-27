#include "floorplan/SortedMacros.h"
#include <algorithm>
#include "floorplan/Macro.h"

SortedMacros::SortedMacros(bool sortByWidth) {
    macros = new std::list<Macro *>();
    this->sortByWidth = sortByWidth;
}

SortedMacros::SortedMacros(SortedMacros *sortedMacros) {
    macros = new std::list<Macro *>(*sortedMacros->getMacros());
    sortByWidth = sortedMacros->doesSortByWidth();
}

SortedMacros::~SortedMacros() {
    delete macros;
}

bool SortedMacros::doesSortByWidth() {
    return sortByWidth;
}

void SortedMacros::insert(Macro *macro) {
    if (macros->size() == 0) {
        macros->push_back(macro);
        return;
    }
    if (sortByWidth) {
        for (std::list<Macro *>::iterator it = macros->begin(); it != macros->end(); ++it) {
            if ((*it)->getWidth() >= macro->getWidth()) {
                macros->insert(it, macro);
                return;
            }
        }
        // All existing Macros are smaller. Push back macro.
        macros->push_back(macro);
    } else {
        for (std::list<Macro *>::iterator it = macros->begin(); it != macros->end(); ++it) {
            if ((*it)->getHeight() >= macro->getHeight()) {
                macros->insert(it, macro);
                return;
            }
        }
        // All existing Macros are smaller. Push back macro.
        macros->push_back(macro);
    }
}

void SortedMacros::erase(Macro *macro) {
    //for (std::list<Macro *>::iterator it = macros->begin(); it != macros->end(); ++it) {
    //    if (*it == macro) {
    //        macros->erase(it);
    //        return;
    //    }
    //}
    macros->erase(std::find(macros->begin(), macros->end(), macro));
}

std::list<Macro *> *SortedMacros::getMacros() {
    return macros;
}

Macro *SortedMacros::getSmallest() {
    if (macros->size() > 0) {
        return macros->front();
    } else {
        return 0;
    }
}

void SortedMacros::print() {
    for (std::list<Macro *>::iterator it = macros->begin(); it != macros->end(); ++it) {
        (*it)->print();
    }
}