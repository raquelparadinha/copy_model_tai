#include <iostream>
#include <unistd.h>

#include "Reader.h"
#include "CopyModel.h"

void printUsage() { 
    std::cout << "\nUsage: ./cpm [options]\n"
              << "\n"
              << "Options:\n"
              << "  -h            Show this help message and exit\n"
              << "  -f <file>     Specify the file to process\n"
              << "  -k <int>      Specify the size of the K Strings\n"
              << "  -t <double>   Specify the threshold value\n"
              << "  -a <double>   Specify the smothing paramenter (alpha) value\n"
              << "  -w <int>      Specify the size of the fallback window\n"
              << std::endl;
}


int main(int argc, char* argv[]) {
    std::string filename = "../example/chry.txt";
    int k = 3;
    double threshold = 0.3;
    double alpha = 1;
    int fallbackWindowSize = 200;

    // Parse command line arguments
    int opt;
    while((opt = getopt(argc, argv, "f:k:t:a:w:h")) != -1) {
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
        case 'h':
            printUsage();
            return 0;
        case '?':
            printUsage();
            return 1;
        default:
            printUsage();
            return 1;
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

