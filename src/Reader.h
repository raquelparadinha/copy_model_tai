// Reader.h
#ifndef READER_H
#define READER_H

#include <unordered_map>
#include <vector>
#include <string>


class Reader {
private:
    std::string filePath;
    std::string fileContent; 
    std::vector<char> alphabet;
    std::unordered_map<char, int> frequencies;

public:
    // Constructor
    Reader(const std::string& filePath);

    // Function to read the file byte-wise and compute frequencies
    void readFile();

    std::string getContent();

    std::vector<char> getAlphabet();
};

#endif // READER_H
