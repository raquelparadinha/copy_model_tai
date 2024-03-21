#include <fstream>
#include <iostream>
#include <vector>
#include <cmath> 
#include <algorithm> 
#include <list>
#include <numeric> 
#include <unistd.h>
#include "Reader.h"


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
    int fileSize = reader.getFileSize();

    std::vector<char> predictedText;
    std::string currentKString = "";
    std::unordered_map<std::string, std::vector<int>> kStringPositions = reader.getKStringsPositions(k);
    std::unordered_map<char, double> symbolProb;
    std::unordered_map<char, std::array<int, 2>> symbolStats;

    std::vector<char> keys;
    for (const auto& pair : frequencies) {
        keys.push_back(pair.first);
    }

    for (int i = 0; i < keys.size(); ++i) {
        symbolStats[keys[i]] = {0,0};  // Assign value (i squared) to the key i
    }

    int hits = 0, tries = 0;
    double totalBits = 0.0, bits;
    int pointer = 0; 
    bool copyModel = true;
    double hitRate;
    double comp_prob;

    char predicted;

    for (int i = 0; i < fileSize; i++) {
        if (i < k) {
            // predicted = reader.getRandomSymbol();

        } else {
            if (copyModel && kStringPositions.find(currentKString) == kStringPositions.end()) { 
                kStringPositions[currentKString] = {i};
                // predicted = reader.getRandomSymbol(); 
            } else if (copyModel) {
                pointer = kStringPositions[currentKString].front();
                predicted = predictedText[pointer + 1];
                kStringPositions[currentKString].push_back(i);
            } else { 
                predicted = predictedText[++pointer];
                kStringPositions[currentKString].push_back(i);
            }
            currentKString.erase(0, 1);
        }
        currentKString += predicted;
        std::cout << "K String: " << currentKString << std::endl;
        predictedText.push_back(predicted);

        tries++;
        if (predicted == originalText[i]) {
            hits++;
            symbolStats[predicted][0]++;
        } else {
            copyModel = copyModel && hits + threshold < tries;
            symbolStats[predicted][1]++;
        }
        //std::cout << "Hits: " << hits << " Tries: " << tries << std::endl;

        hitRate = (hits + alpha) / (tries + 2 * alpha);
        //std::cout << "Hit rate: " << hitRate << std::endl;

        symbolProb[predicted] = (hits + alpha) / (tries + 2 * alpha);
        comp_prob = (1 - symbolProb[predicted]) / 3;
        bits = -log2(symbolProb[predicted]);
        totalBits += bits;
    }
    std::cout << "Predicted text: " << std::endl;
    for (int i = 0; i < predictedText.size(); i++) {
        std::cout << predictedText[i];
    }
    std::cout << std::endl;

        for (const auto& pair : symbolStats) {
        std::cout << "Key: " << pair.first << ", Values: ";
        for (const auto& value : pair.second) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }


    double prob;
    int nHits;
    int nMisses;
    for(const auto& pair : symbolStats) {
        std::cout << "Probability of Char " << pair.first << " " << std::endl;
        nHits = pair.second[0];
        nMisses = pair.second[1];
        if (nHits == 0) {
            prob = static_cast<double>(nHits + alpha) / (nHits + nMisses + 2 * alpha);
        }
        else {
            prob = static_cast<double>(nHits) / nMisses;
        }
        std::cout << "Prob: " << prob << std::endl;
    }

    return 0;
}
