#include <fstream>
#include <iostream>
#include <vector>
#include <cmath> 
#include <algorithm> 
#include <list>
#include <numeric> 
#include <bits/getopt_core.h>
#include <random>

using namespace std;

// Function to calculate the knowned probability distribution
unordered_map<char, float> getOriginalProbs(vector<char> originalText) {
    unordered_map<char, int> symbols;
    unordered_map<char, float> probabilities;

    for (int i = 0; i < originalText.size(); i++) {
        char symbol = originalText[i];
        if (symbols.find(symbol) == symbols.end()) {
            symbols[symbol] = 1;
        } else {
            symbols[symbol]++;
        }
    }

    for (auto it = symbols.begin(); it != symbols.end(); it++) {
        probabilities[it->first] = (float)it->second / originalText.size();
    }

    return probabilities;
}

unsigned char selectSymbol(const unordered_map<char, float>& probabilities) {
    // Step 1: Create a cumulative distribution
    unordered_map<char, float> cumulativeProbabilities(probabilities.size());
    float sum = 0.0f;
    for (const auto& pair : probabilities) {
        sum += pair.second;
        cumulativeProbabilities[pair.first] = sum;
    }
    cout << "Cumulative probabilities: " << endl;
    for (auto it = cumulativeProbabilities.begin(); it != cumulativeProbabilities.end(); it++) {
        cout << it->first << " " << it->second << endl;
    }

    // Step 2: Generate a random number between 0 and 1
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);
    double randomValue = dis(gen);

    // Step 3: Find the symbol corresponding to the random value
    for (int i = 0; i < cumulativeProbabilities.size(); ++i) {
        if (randomValue <= cumulativeProbabilities[i]) {
            cout << "Random value: " << randomValue << " Selected symbol: " << i << endl;
            return i;
        }
    }
    cout << "dif Random value: " << randomValue << " Selected symbol: " << static_cast<unsigned char>(probabilities.size() - 1) << endl;
    // Fallback in case of rounding errors, should not be reached
    return static_cast<unsigned char>(probabilities.size() - 1);
}

int main(int argc, char* argv[]) {
    const char* filename = argv[1];
    int k = 4;
    float threshold = 0.5; 
    double alpha = 0.01;

    // // Parse command line arguments
    // int opt;
    // while((opt = getopt(argc, argv, "f:k:t:a:")) != -1) {
    //     switch (opt)
    //     {
    //     case 'f':
    //         filename = optarg;
    //         break;
    //     case 'k':
    //         k = atoi(optarg);
    //         break;
    //     case 't':
    //         threshold = atof(optarg);
    //         break;
    //     case 'a':
    //         alpha = atof(optarg);
    //         break;
    //     default:
    //         cerr << "Usage: " << argv[0] << " -f <string> -k <int> -t <float> -a <double>" << endl;
    //         break;
    //     }

    // }


    // Open the file
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return 1;
    }
    
    vector<char> originalText; // Vector to store the symbols read from the file

    // Read the file into a vector
    char symbol;
    while (file.read(&symbol, 1)) {
        originalText.push_back(symbol);
    }
    file.close();

    vector<char> predictedText;
    // vector<string> kStrings;
    string currentKString = "";
    unordered_map<string, list<int>> kStringPositions;

    int hits = 0, misses = 0;
    double totalBits = 0.0;
    int pointer; 
    bool searchTable = true;

    unordered_map<char, float> initialProbs = getOriginalProbs(originalText);
    char predicted;

    for (int i = 0; i < originalText.size(); i++) {
        if (i < k) {
            predicted = selectSymbol(initialProbs);

        } else {
            if (searchTable && kStringPositions.find(currentKString) == kStringPositions.end()) {
                kStringPositions[currentKString] = {i};
                predicted = selectSymbol(initialProbs);
                pointer = i - 1;
                
            } else if (searchTable) {
                pointer = kStringPositions[currentKString].front();
                predicted = predictedText[pointer + 1];
                kStringPositions[currentKString].push_back(i);
            } else {
                predicted = predictedText[pointer + 1];
                kStringPositions[currentKString].push_back(i);
            }
            currentKString.erase(0, 1);
        }
        currentKString += predicted;
        predictedText.push_back(predicted);

        if (predicted == originalText[i]) {
            ++hits;
        } else {
            ++misses;
        }

        double hitRate = (hits + alpha) / (hits + misses + 2 * alpha);

        if (hitRate < threshold && i >= k) {
            searchTable = true;
            if (!kStringPositions[currentKString].empty()) {
                kStringPositions[currentKString].pop_front();
            }
        } else {
            searchTable = false;
        }
    }
    cout << "Predicted text: " << endl;
    for (int i = 0; i < predictedText.size(); i++) {
        cout << predictedText[i];
    }

    return 0;
}


    // for (size_t i = 1; i < originalText.size(); ++i) {
    //     // Predict the next symbol based on the pointer's current position
    //     unsigned char predicted = originalText[pointer]; // Prevent underflow
    //     unsigned char actual = originalText[i];

    

    //     // Calculate the current hit rate
    //     double hitRate = static_cast<double>(hits) / (hits + misses);

    //     // Reposition the pointer if performance drops below the threshold
    //     if (hitRate < threshold && i >= k) {
    //         bool found = false;
    //         // Search for a matching sequence in the last k symbols
    //         for (size_t j = 0; j <= i - k; ++j) {
    //             if (std::equal(originalText.begin() + j, originalText.begin() + j + k, originalText.begin() + i - k + 1)) {
    //                 pointer = j;
    //                 found = true;
    //                 break;
    //             }
    //         }
    //         if (!found) {
    //             pointer = i - 1; // Reset to the most recent symbol if no match is found
    //         }
    //     }

    //     // Estimate the amount of information needed for this symbol
    //     double pHit = (hits + alpha) / (hits + misses + 2 * alpha);
    //     double bitsForSymbol = -log2(pHit);
    //     totalBits += bitsForSymbol;
    // }

    // Output results
    // std::cout << "Hits: " << hits << ", Misses: " << misses << std::endl;
    // std::cout << "Estimated total bits: " << totalBits << std::endl;
    // std::cout << "Average bits per symbol: " << totalBits / originalText.size() << std::endl;