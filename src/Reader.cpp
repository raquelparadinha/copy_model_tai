#include "Reader.h"
#include <algorithm>
#include <cctype>
#include <numeric>
#include <random>

Reader::Reader(const std::string& filePath) : filePath(filePath) {}

void Reader::readFile() {
    std::ifstream file(filePath, std::ios::binary); // Open the file in binary mode

    if (!file.is_open()) {
        std::cerr << "Could not open the file - '" << filePath << "'" << std::endl;
        return;
    }

    fileContent.clear(); // Clear previous content
    frequencies.clear(); // Reset frequencies
    totalLetters = 0;    // Reset letter count
    char byte;
    
    while (file.get(byte)) { // Read byte by byte
        if (isalpha(static_cast<unsigned char>(byte))) { // Check if the byte is a letter
            char lowerByte = tolower(static_cast<unsigned char>(byte));
            fileContent += lowerByte; // Store the lowercase letter in fileContent
            frequencies[lowerByte]++;
            totalLetters++;
        }
    }
    file.close();
}

std::vector<std::pair<char, double>> Reader::computeFrequencies() {
    std::vector<std::pair<char, double>> relativeFrequencies;
    
    if (totalLetters == 0) {
        std::cout << "No letters found or file not read." << std::endl;
        return relativeFrequencies;
    }

    for (const auto& entry : frequencies) {
        double relativeFrequency = static_cast<double>(entry.second) / totalLetters;
        relativeFrequencies.emplace_back(entry.first, relativeFrequency);
    }
    
    // Sort the vector by relative frequency in descending order
    std::sort(relativeFrequencies.begin(), relativeFrequencies.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    
    for (const auto& [letter, freq] : relativeFrequencies) {
        std::cout << "Letter: " << letter << ", Relative Frequency: " << freq << std::endl;
    }
    
    return relativeFrequencies;
}

std::string Reader::getContent() {
    return fileContent;
}

char Reader::getRandomSymbol() {
    if (cumulativeFrequencies.empty() && !frequencies.empty()) {
        double sum = 0;
        for (const auto& entry : frequencies) {
            sum += static_cast<double>(entry.second) / totalLetters;
            cumulativeFrequencies.emplace_back(entry.first, sum);
        }
    }

    // Early exit if no frequencies
    if (cumulativeFrequencies.empty()) {
        std::cerr << "No data to select from." << std::endl;
        return '\0'; // Indicate failure/no selection
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    double randomValue = dis(gen);

    for (const auto& entry : cumulativeFrequencies) {
        if (randomValue <= entry.second) {
            return entry.first;
        }
    }

    // Fallback, should not normally reach here
    return cumulativeFrequencies.back().first;
}

int Reader::getTotalLetters() {
    return totalLetters;
}
