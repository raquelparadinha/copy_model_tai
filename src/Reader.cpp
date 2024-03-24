#include "Reader.h"
#include <algorithm>
#include <cctype>
#include <numeric>
#include <random>

Reader::Reader(const std::string& filePath) : filePath(filePath) {
    readFile();
}

void Reader::readFile() {
    std::ifstream file(filePath, std::ios::binary); 

    if (!file) {
        std::cerr << "Could not open the file - '" << filePath << "'" << std::endl;
        return;
    }

    fileContent.clear(); 
    kStringsPositions.clear();
    frequencies.clear(); // Reset frequencies
    fileSize = 0;
    alphabetSize = 0;
    alphabet.clear();

    char ch;
    while (file.get(ch)) { 
        if (isalpha(static_cast<unsigned char>(ch))) 
        { 
            char lowerCh = tolower(static_cast<unsigned char>(ch));
            fileContent += lowerCh; 
            fileSize++;
            if (std::find(alphabet.begin(), alphabet.end(), lowerCh) == alphabet.end())
            {
                alphabet.push_back(lowerCh);
                alphabetSize++;
            }            
        }
    }
    std::cout << "Alphabet: " << std::endl;
    for (const auto& letter : alphabet) {
        std::cout << letter << " ";
    }
    std::cout << std::endl;
    std::cout << "Size: " << getAlphabetSize() << std::endl; 

    file.close();
}

std::vector<std::pair<char, double>> Reader::computeFrequencies() {
    std::vector<std::pair<char, double>> relativeFrequencies;
    
    if (fileSize == 0) {
        std::cout << "No letters found or file not read." << std::endl;
        return relativeFrequencies;
    }

    for (const auto& entry : frequencies) {
        double relativeFrequency = static_cast<double>(entry.second) / fileSize;
        relativeFrequencies.emplace_back(entry.first, relativeFrequency);
    }
    
    // // Sort the vector by relative frequency in descending order
    // std::sort(relativeFrequencies.begin(), relativeFrequencies.end(), [](const auto& a, const auto& b) {
    //     return a.second > b.second;
    // });
    
    // for (const auto& [letter, freq] : relativeFrequencies) {
    //     std::cout << "Letter: " << letter << ", Relative Frequency: " << freq << std::endl;
    // }
    
    return relativeFrequencies;
}

std::string Reader::getContent() {
    return this->fileContent;
}

std::unordered_map<std::string, std::vector<int>> Reader::getKStringsPositions(int kStringSize) {
    if (fileContent.empty()) {
        std::cerr << "No content found or file not read." << std::endl;
    }

    for (int i = 0; i < fileSize - kStringSize + 1; ++i) {
        std::string kString = fileContent.substr(i, kStringSize);
        kStringsPositions[kString].push_back(i);
    }

    return this->kStringsPositions;
}

int Reader::getAlphabetSize() {
    return this->alphabetSize;
}

std::vector<char> Reader::getAlphabet() {
    return this->alphabet;
}

int Reader::getFileSize() {
    return this->fileSize;
}
