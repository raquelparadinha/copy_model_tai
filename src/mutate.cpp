#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

std::unordered_set<char> constructAlphabet(const std::string& filename) {
    std::ifstream file(filename);
    std::unordered_set<char> alphabet;
    char ch;

    while (file.get(ch)) {
        alphabet.insert(ch);
    }

    file.close();
    return alphabet;
}

void mutateFile(const std::string& inputFilename, const std::string& outputFilename, double mutationProbability, const std::unordered_set<char>& alphabet) {
    std::ifstream inputFile(inputFilename);
    std::ofstream outputFile(outputFilename);
    
    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::vector<char> alphabetVec(alphabet.begin(), alphabet.end()); // Convert to vector for easy random access

    char currentChar;
    while (inputFile.get(currentChar)) {
        if (dis(gen) < mutationProbability) {
            // Mutation happens
            std::uniform_int_distribution<> mutationDis(0, alphabetVec.size() - 1);
            char newChar;
            do {
                newChar = alphabetVec[mutationDis(gen)];
            } while (newChar == currentChar); // Ensure we actually mutate to a different character
            currentChar = newChar;
        }
        outputFile << currentChar;
    }

    inputFile.close();
    outputFile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file> <mutation probability>" << std::endl;
        return 1;
    }

    std::string inputFilename = argv[1];
    std::string outputFilename = argv[2];
    double mutationProbability = std::stod(argv[3]);

    auto alphabet = constructAlphabet(inputFilename);
    mutateFile(inputFilename, outputFilename, mutationProbability, alphabet);

    return 0;
}
