#ifndef COPY_MODEL_H
#define COPY_MODEL_H

#include "Reader.h"

class CopyModel {
private:
    int k;
    double threshold;
    int alpha;
    int alphabet_size;
    Reader reader;
    std::vector<std::string> pastKStrings;
    std::string predictedText;





public:
    CopyModel(int k, double threshold, int alpha, int alphabet_size, std::string file);

    void runCopyModel();

    

};

#endif