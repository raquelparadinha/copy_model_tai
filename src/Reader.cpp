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

std::string Reader::getContent() {
    return this->fileContent;
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
