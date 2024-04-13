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

public:
    Reader(const std::string& filePath);

    void readFile();

    std::string getContent();

    std::vector<char> getAlphabet();
};

#endif 
