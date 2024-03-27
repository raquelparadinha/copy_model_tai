#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip> // For std::setw
#include <limits>
#include <map>
#include "Reader.h"
#include "CopyModel.h"
#include "Stats.h"

void runAndSaveResultsForK(const std::string& originalText, const std::vector<char>& alphabet, double referenceThreshold, double referenceAlpha, int referenceFallbackWindowSize) {
    std::ofstream outFile("../output/results/results_k.json");
    outFile << "[" << std::endl; // Begin JSON array
    
    bool firstEntry = true;
    for (int k = 3; k <= 15; ++k) {
        if (!firstEntry) {
            outFile << "," << std::endl;
        }
        firstEntry = false;
        
        // Setup and run the CopyModel
        CopyModel copyModel(originalText, alphabet, k, referenceThreshold, referenceAlpha, referenceFallbackWindowSize);
        std::map<std::string, double> results;
        results = copyModel.run();


        // Prepare JSON entry for current configuration
        outFile << "  {" << std::endl;
        outFile << "    \"k\": " << k << "," << std::endl;
        outFile << "    \"totalNumberOfBits\": " << results["totalBits"] << "," << std::endl;
        outFile << "    \"executionTime\": " << results["executionTime"] << "," << std::endl;
        outFile << "    \"averageBitsPerSymbol\": " << results["averageBitsPerSymbol"] << "," << std::endl;
        outFile << "    \"compressionRatio\": " << results["compressionRatio"] << std::endl;
        outFile << "  }";
    }

    outFile << std::endl << "]" << std::endl; // End JSON array
    outFile.close();
}

void runAndSaveResultsForT(const std::string& originalText, const std::vector<char>& alphabet, double referenceK, double referenceAlpha, int referenceFallbackWindowSize) {
    std::ofstream outFile("../output/results/results_t.json");
    outFile << "[" << std::endl; // Begin JSON array
    
    bool firstEntry = true;
    for (double t = 0.1; t <= 0.45; t+=0.05) {
        if (!firstEntry) {
            outFile << "," << std::endl;
        }
        firstEntry = false;
        
        // Setup and run the CopyModel
        CopyModel copyModel(originalText, alphabet, referenceK, t, referenceAlpha, referenceFallbackWindowSize);
        std::map<std::string, double> results;
        results = copyModel.run();

        // Prepare JSON entry for current configuration
        outFile << "  {" << std::endl;
        outFile << "    \"threshold\": " << t << "," << std::endl;
        outFile << "    \"totalNumberOfBits\": " << results["totalBits"] << "," << std::endl;
        outFile << "    \"executionTime\": " << results["executionTime"] << "," << std::endl;
        outFile << "    \"averageBitsPerSymbol\": " << results["averageBitsPerSymbol"] << "," << std::endl;
        outFile << "    \"compressionRatio\": " << results["compressionRatio"] << std::endl;
        outFile << "  }";
    }

    outFile << std::endl << "]" << std::endl; // End JSON array
    outFile.close();
}

void runAndSaveResultsForA(const std::string& originalText, const std::vector<char>& alphabet, double referenceThreshold, double referencek, int referenceFallbackWindowSize) {
    std::ofstream outFile("../output/results/results_a.json");
    outFile << "[" << std::endl; // Begin JSON array
    
    bool firstEntry = true;
    for (double a = 0.5; a <= 2.0; a+=0.25) {
        if (!firstEntry) {
            outFile << "," << std::endl;
        }
        firstEntry = false;
        
        // Setup and run the CopyModel
        CopyModel copyModel(originalText, alphabet, referencek, referenceThreshold, a, referenceFallbackWindowSize);
        std::map<std::string, double> results;
        results = copyModel.run();


        // Prepare JSON entry for current configuration
        outFile << "  {" << std::endl;
        outFile << "    \"alpha\": " << a << "," << std::endl;
        outFile << "    \"totalNumberOfBits\": " << results["totalBits"] << "," << std::endl;
        outFile << "    \"executionTime\": " << results["executionTime"] << "," << std::endl;
        outFile << "    \"averageBitsPerSymbol\": " << results["averageBitsPerSymbol"] << "," << std::endl;
        outFile << "    \"compressionRatio\": " << results["compressionRatio"] << std::endl;
        outFile << "  }";
    }

    outFile << std::endl << "]" << std::endl; // End JSON array
    outFile.close();
}

void runAndSaveResultsForWS(const std::string& originalText, const std::vector<char>& alphabet, double referenceThreshold, double referenceAlpha, int referenceK) {
    std::ofstream outFile("../output/results/results_ws.json");
    outFile << "[" << std::endl; // Begin JSON array
    
    bool firstEntry = true;
    for (int ws = 150; ws <= 300; ws+=25) {
        if (!firstEntry) {
            outFile << "," << std::endl;
        }
        firstEntry = false;
        
        // Setup and run the CopyModel
        CopyModel copyModel(originalText, alphabet, referenceK, referenceThreshold, referenceAlpha, ws);
        std::map<std::string, double> results;
        results = copyModel.run();


        // Prepare JSON entry for current configuration
        outFile << "  {" << std::endl;
        outFile << "    \"windowSize\": " << ws << "," << std::endl;
        outFile << "    \"totalNumberOfBits\": " << results["totalBits"] << "," << std::endl;
        outFile << "    \"executionTime\": " << results["executionTime"] << "," << std::endl;
        outFile << "    \"averageBitsPerSymbol\": " << results["averageBitsPerSymbol"] << "," << std::endl;
        outFile << "    \"compressionRatio\": " << results["compressionRatio"] << std::endl;
        outFile << "  }";
    }

    outFile << std::endl << "]" << std::endl; // End JSON array
    outFile.close();
}

// Similar functions for threshold, alpha, and fallbackWindowSize can be defined here
// Example:
// void runAndSaveResultsForThreshold(const std::string& originalText, const std::vector<char>& alphabet, int referenceK, double referenceAlpha, int referenceFallbackWindowSize) {...}

int main(int argc, char* argv[]) {
    std::string filename = "../example/chry.txt"; // Adjust as necessary

    // Load original text and alphabet
    Reader reader(filename);
    reader.readFile();
    std::string originalText = reader.getContent();
    std::vector<char> alphabet = reader.getAlphabet();

    // Reference values for other parameters
    int referenceK = 12;
    double referenceThreshold = 0.3;
    double referenceAlpha = 1.0;
    int referenceFallbackWindowSize = 225;

    // Run parameter tuning for k and save results
    runAndSaveResultsForK(originalText, alphabet, referenceThreshold, referenceAlpha, referenceFallbackWindowSize);

    runAndSaveResultsForT(originalText, alphabet, referenceK, referenceAlpha, referenceFallbackWindowSize);

    runAndSaveResultsForA(originalText, alphabet, referenceThreshold, referenceK, referenceFallbackWindowSize);

    runAndSaveResultsForWS(originalText, alphabet, referenceThreshold, referenceAlpha, referenceK);


    // Note: You can call similar functions here for threshold, alpha, and fallbackWindowSize,
    // each saving its results to a different JSON file.

    return 0;
}
