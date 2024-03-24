#ifndef COPY_MODEL_H
#define COPY_MODEL_H

#include "Reader.h"


class CopyModel {
private:
    int k;
    double threshold;
    int alpha;
    int globalPointer;
    int copyPointer;
    std::vector<std::string> pastKStrings;
    std::string predictedText;
    std::string currentKString;

    std::string originalText;
    std::unordered_map<std::string, std::vector<int>> kStringsPositions;
    int alphabetSize;

    int fallbackWindowSize;

    double totalNumberOfBits;

public:
    CopyModel(int k, double threshold, int alpha, std::string originalText, std::unordered_map<std::string, std::vector<int>> kStringsPositions, int alphabetSize, int fallbackWindowSize);

    void run();
    void copyModel();
    void incrementGlobalPointer();
    std::string findCopyModel();

    void fallbackModel();   
};

#endif