#include "Stats.h"

/**
 * @brief Constructs a Stats object.
 */

Stats::Stats(double a) : alpha(a)
{
    this->numberOfHits = 0;
    this->numberOfMisses = 0;
}

void Stats::incrementHits()
{
    numberOfHits++;
}

void Stats::incrementMisses()
{
    numberOfMisses++;
}

int Stats::getNumberOfHits() const
{
    return numberOfHits;
}

int Stats::getNumberOfMisses() const
{
    return numberOfMisses;
}

double Stats::getProbability()
{
    return (double)(numberOfHits + alpha) / (numberOfHits + numberOfMisses + 2 * alpha);
}
