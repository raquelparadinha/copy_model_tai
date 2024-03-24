#include "CopyModel.h"
#include "Reader.h"
#include "Stats.h"
#include <algorithm>
#include <cmath>
#include <map>

/**
 * @brief Constructs a CopyModel object.
 *
 * @param k The value of k.
 * @param t The value of threshold.
 * @param a The value of alpha.
 * @param oT The value of originalText.
 * @param kStringsPos The value of kStringsPositions.
 * @param size The value of alphabetSize.
 */
CopyModel::CopyModel(int k, double t, int a, std::string oT, std::unordered_map<std::string, std::vector<int>> kStringsPos, std::vector<char> alphabet, int window) : 
    k(k), threshold(t), alpha(a), originalText(oT), kStringsPositions(kStringsPos), alphabet(alphabet), fallbackWindowSize(window)
{
    this->pastKStrings = std::vector<std::string>();
    this->alphabetSize = alphabet.size();

    this->globalPointer = 0;
    this->copyPointer = 0;

    this->currentKString = "";
    this->predictedText = "";
    
    this->totalNumberOfBits = 0;
}

/**
 * Runs the CopyModel.
 *
 * corre esta merda toda
 *
 */
void CopyModel::run()
{
    std::cout << "Text size: " << originalText.size() << std::endl;
    std::cout << "Finding first window...";
    std::string match = findCopyModel();

    while (globalPointer < (int)originalText.size() - k) // break when reaches the last kString
    {
        std::cout << std::endl;
        if (match != "")
        {
            std::cout << "Copy modelling..." << std::endl;
            copyModel();
        }
        else
        {
            std::cout << "Fallback mode..." << std::endl;
            fallbackModel();
        }
        std::cout << "Finding next window...";
        match = findCopyModel();
    }

    std::cout << std::endl << std::endl << "--=[     ]=--" << std::endl;
    std::cout << "Total number of bits: " << (double)totalNumberOfBits << std::endl;
    std::cout << "Average number of bits per symbol: " << (double)(totalNumberOfBits / originalText.size()) << std::endl;
    std::cout << "Compression ratio: " << (double)originalText.size() / totalNumberOfBits << std::endl;
    std::cout << "--=[     ]=--" << std::endl;
}


/**
 * Runs the CURRENT MATCHING WINDOW.
 *
 * Checks the next symbols and compares them to the ones found previously. Stops when probability falls below threshold
 *
 * quando tens um copy model, vai copiando dessa janela até a probabilidade de predict bom ser menor que o threshold
 *
 */
void CopyModel::copyModel()
{
    // Inicializa as stats e o threshold
    Stats stats(alpha);

    // Compara o simbolo da posição atual com o simbolo da posição do copy model anterior
    double prediction = stats.getProbability();
    std::cout << "Initial prediction: " << prediction << std::endl;
    while (prediction < threshold)
    {
        if (originalText[copyPointer] == originalText[globalPointer])
        {
            std::cout << "Match: " << originalText[copyPointer] << " == " << originalText[globalPointer] << std::endl;
            stats.incrementHits();
            this->totalNumberOfBits += -std::log2(prediction);
        }
        else
        {
            std::cout << "Miss: " << originalText[copyPointer] << " != " << originalText[globalPointer] << std::endl;
            stats.incrementMisses();
            double comp_prediction = 1 - prediction;
            this->totalNumberOfBits += -std::log2(comp_prediction / (alphabetSize - 1));
        }

        // std::cout << "Hits: " << stats.getNumberOfHits() << ", " << "Misses: " << stats.getNumberOfMisses() << std::endl;

        incrementGlobalPointer();
        copyPointer++;

        prediction = stats.getProbability();
        std::cout << "Next prediction: " << prediction << std::endl;
    }
    // Exit loop meaning the copy model accuracy is too low
}

/**
 * Attemps to find a copy model for the current kstring.
 *
 * This function searches for repeated kStrings in the predicted text to start a Copy Model run.
 *
 * @return The first repeated kString found or an empty string if none is found.
 */
std::string CopyModel::findCopyModel()
{
    currentKString = originalText.substr(globalPointer, k);
    if (std::find(pastKStrings.begin(), pastKStrings.end(), currentKString) != pastKStrings.end())
    {
        // Found a matching window
        copyPointer = kStringsPositions[currentKString].front();
        std::cout << " Found a model." << std::endl;

        return currentKString;
    }

    std::cout << " No copy found." << std::endl;
    return "";
}

void CopyModel::fallbackModel()
{   
    double prob;
    if (globalPointer < fallbackWindowSize)
    {
        currentKString = originalText.substr(globalPointer, k);
        for (char c : currentKString.substr(0, 1))
        {
            prob = 1 / (double)alphabetSize;
            std::cout << "1 number of bits before: " << totalNumberOfBits << std::endl;
            this->totalNumberOfBits += -std::log2(prob);
            std::cout << "1 number of bits after: " << totalNumberOfBits << std::endl;
            incrementGlobalPointer();
        }
    }
    else
    {
        std::string fallbackString = originalText.substr(globalPointer - fallbackWindowSize, fallbackWindowSize);
        currentKString = originalText.substr(globalPointer, k);

        std::map<char, int> charCounts;
        std::map<char, double> charProbabilities;
        for (char c : alphabet)
        {
            charCounts[c] = 0;
            charProbabilities[c] = 0;
        }

        std::cout << "Fallback string: " << fallbackString << std::endl;
        for (char c : fallbackString)
        {
            charCounts[c]++;
        }
        for (const auto &pair : charCounts)
        {
            std::cout << "Char: " << pair.first << ", Count: " << pair.second << std::endl;
            charProbabilities[pair.first] = (double)(pair.second + alpha) / (double)(fallbackWindowSize + alphabetSize * alpha);
        }

        for (const auto &pair : charProbabilities)
        {
            std::cout << "Char: " << pair.first << ", Probability: " << pair.second << std::endl;
        }

        for (char c : currentKString.substr(0, 1))
        {
            prob = charProbabilities[c];
            std::cout << "2 number of bits before: " << totalNumberOfBits << std::endl;
            this->totalNumberOfBits += -std::log2(prob);
            std::cout << "2 number of bits after: " << totalNumberOfBits << std::endl;
            incrementGlobalPointer();
        }
    }
}

void CopyModel::incrementGlobalPointer()
{
    globalPointer++;
    predictedText += originalText[globalPointer];
    pastKStrings.push_back(currentKString);
    std::cout << " + global pointer " << globalPointer << std::endl;
}