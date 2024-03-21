// Reader.h
#ifndef READER_H
#define READER_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>


class Reader {
private:
    std::string filePath;
    std::string fileContent; 
    std::unordered_map<std::string, std::vector<int>> kStringsPositions;
    std::vector<char> alphabet;
    int alphabetSize;
    int fileSize;
    std::unordered_map<char, int> frequencies;

public:
    // Constructor
    Reader(const std::string& filePath);

    Reader();

    // Function to read the file byte-wise and compute frequencies
    void readFile();

    std::string getContent();

    std::unordered_map<std::string, std::vector<int>> getKStringsPositions(int KStringSize);

    int getAlphabetSize();

    std::vector<char> getAlphabet();

    int getFileSize();

    std::vector<std::pair<char, double>> computeFrequencies();

};

#endif // READER_H
