#include <fstream>
#include <iostream>
#include <vector>
#include <cmath> 
#include <algorithm> 
#include <list>
#include <numeric> 
#include <unistd.h>
#include "Reader.h"
#include "CopyModel.h"


int main(int argc, char* argv[]) {
    std::string filename = "../example/test.txt";
    int k = 3;
    double threshold = 0.7;
    int alpha = 1;
    int fallbackWindowSize = 2;

    // Parse command line arguments
    int opt;
    while((opt = getopt(argc, argv, "f:k:t:a:")) != -1) {
        switch (opt)
        {
        case 'f':
            filename = optarg;
            break;
        case 'k':
            k = atoi(optarg);
            break;
        case 't':
            threshold = atof(optarg);
            break;
        case 'a':
            alpha = atoi(optarg);
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " -f <string> -k <int> -t <double> -a <int>" << std::endl;
            break;
        }
    }

    std::cout << "Reading file... " << std::endl;

    Reader reader(filename);
    reader.readFile();
    // std::vector<std::pair<char, double>> frequencies = reader.computeFrequencies();
    std::string originalText = reader.getContent();
    // int fileSize = reader.getFileSize();

    std::unordered_map<std::string, std::vector<int>> kStringPositions = reader.getKStringsPositions(k);
    // std::unordered_map<char, double> symbolProb;
    // std::unordered_map<char, std::array<int, 2>> symbolStats;

    // std::vector<char> keys;
    
    std::cout << "Running CopyModel... " << std::endl;
    CopyModel copyModel(k, threshold, alpha, originalText, kStringPositions, reader.getAlphabetSize(), fallbackWindowSize);
    copyModel.run();

    return 0;
}
