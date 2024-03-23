#ifndef STATS_H
#define STATS_H

class Stats {
private:
    int numberOfHits;
    int numberOfMisses;

public:
    Stats();

    void incrementHits();
    void incrementMisses();

    int getNumberOfHits() const;
    int getNumberOfMisses() const;

    double getProbability();

};

#endif