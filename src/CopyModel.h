#ifndef COPY_MODEL_H
#define COPY_MODEL_H

#include "Reader.h"


class CopyModel {
private:
    std::string originalText;
    std::vector<char> alphabet;

    int k;
    double threshold;
    double alpha;
    int fallbackWindowSize;

    int globalPointer;
    int copyPointer;
    int alphabetSize;
    std::unordered_map<std::string, std::vector<int>> pastKStrings;

    std::string predictedText;
    std::string currentKString;

    double totalNumberOfBits;
public:
    CopyModel(std::string originalText, std::vector<char> alphabet, int k, double threshold, double alpha, int fallbackWindowSize);

    void run();

    std::string findCopyModel();
    void copyModel();
    void checkKString(int pointer, int local_pointer);
    void startThread(int pointer, int local_pointer); 
    void fallbackModel();
    void incrementGlobalPointer();
};

#endif