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
    std::string filename = "../example/chry.txt";
    int k = 3;
    double threshold = 0.3;
    double alpha = 1;
    int fallbackWindowSize = 200;

    // Parse command line arguments
    int opt;
    while((opt = getopt(argc, argv, "f:k:t:a:w:")) != -1) {
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
            alpha = atof(optarg);
            break;
        case 'w':
            fallbackWindowSize = atoi(optarg);
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " -f <string> -k <int> -t <double> -a <double> -w <int>" << std::endl;
            break;
        }
    }


    std::cout << std::endl << "Reading file... " << std::endl;
    Reader reader(filename);
    std::string originalText = reader.getContent();
    std::vector<char> alphabet = reader.getAlphabet();


    std::cout << "Text size: " << originalText.size() << std::endl;
    std::cout << "kString size: " << k << std::endl;
    std::cout << "Threshold: " << threshold << std::endl;
    std::cout << "Alpha: " << alpha << std::endl;
    std::cout << "FallbackWindow size: " << fallbackWindowSize << std::endl;
    std::cout << std::endl << "Running CopyModel... " << std::endl;
    CopyModel copyModel(originalText, alphabet, k, threshold, alpha, fallbackWindowSize);
    copyModel.run();


    return 0;
}
