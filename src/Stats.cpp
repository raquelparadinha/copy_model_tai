#include "Stats.h"
#include "Reader.h"
#include <algorithm>

/**
 * @brief Constructs a Stats object.
 */

Stats::Stats() {
    this->numberOfHits = 0;
    this->numberOfMisses = 0;
}

void Stats::incrementHits() {
    numberOfHits++;
}

void Stats::incrementMisses() {
    numberOfMisses++;
}

int Stats::getNumberOfHits() const {
    return numberOfHits;
}

int Stats::getNumberOfMisses() const {
    return numberOfMisses;
}

double Stats::getProbability() {
    //alpha is 1 here
    return (double) (numberOfHits + 1) / (numberOfHits + numberOfMisses + 2*1);
}


