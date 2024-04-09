#include "Reader.h"
#include <fstream>
#include <iostream>
#include <algorithm>

Reader::Reader(const std::string& filePath) : filePath(filePath) {
    readFile();
}

void Reader::readFile() {
    std::ifstream file(filePath, std::ios::binary); 

    if (!file) {
        std::cerr << "Could not open the file - '" << filePath << "'" << std::endl;
        exit(1);
    }

    fileContent.clear(); 
    alphabet.clear();

    char ch;
    while (file.get(ch)) { 
        fileContent += ch; 
        if (std::find(alphabet.begin(), alphabet.end(), ch) == alphabet.end())
        {
            alphabet.push_back(ch);
        }            
    }
    std::cout << "Alphabet: " << std::endl;
    for (const auto& letter : alphabet) {
        std::cout << letter << " ";
    }
    std::cout << std::endl;

    file.close();
}

std::string Reader::getContent() {
    return this->fileContent;
}

std::vector<char> Reader::getAlphabet() {
    return this->alphabet;
}
