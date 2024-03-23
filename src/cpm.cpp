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
    int k = 5;
    int threshold = 3; 
    int alpha = 1;

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
            threshold = atoi(optarg);
            break;
        case 'a':
            alpha = atoi(optarg);
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " -f <string> -k <int> -t <int> -a <int>" << std::endl;
            break;
        }

    }

    Reader reader(filename);
    reader.readFile();
    std::vector<std::pair<char, double>> frequencies = reader.computeFrequencies();
    std::string originalText = reader.getContent();
    // int fileSize = reader.getFileSize();

    std::unordered_map<std::string, std::vector<int>> kStringPositions = reader.getKStringsPositions(k);
    // std::unordered_map<char, double> symbolProb;
    // std::unordered_map<char, std::array<int, 2>> symbolStats;

    // std::vector<char> keys;
    
    CopyModel copyModel(k, threshold, alpha, originalText, kStringPositions, frequencies.size());
    // copyModel.run();
}
