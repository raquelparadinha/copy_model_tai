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
    std::unordered_map<char, int> frequencies;
    std::string filePath;
    std::string fileContent; // To store the content of the file
    int totalLetters = 0;
    std::vector<std::pair<char, double>> cumulativeFrequencies;

public:
    // Constructor
    Reader(const std::string& filePath);

    // Function to read the file byte-wise and compute frequencies
    void readFile();

    // Function to compute and print relative frequencies
    std::vector<std::pair<char, double>> computeFrequencies();

    std::string getContent();

    char getRandomSymbol();

    int getTotalLetters();

};

#endif // READER_H
