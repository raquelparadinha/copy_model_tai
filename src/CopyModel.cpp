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
CopyModel::CopyModel(int k, double t, int a, std::string oT, std::unordered_map<std::string, std::vector<int>> kStringsPos, int alphabetSize, int window) : 
    k(k), threshold(t), alpha(a), originalText(oT), kStringsPositions(kStringsPos), alphabetSize(alphabetSize), fallbackWindowSize(window)
{
    this->pastKStrings = std::vector<std::string>();

    this->globalPointer = 0;
    this->copyPointer = 0;

    this->currentKString = "";
    this->predictedText = "";
    
    this->totalNumberOfBits = 0;
    std::cout << "Initial value of totalNumberOfBits: " << totalNumberOfBits << std::endl;
}

/**
 * Runs the CopyModel.
 *
 * corre esta merda toda
 *
 */
void CopyModel::run()
{
    std::cout << "Finding first window...";
    std::string match = findCopyModel();
    std::cout << "Initial index of Global Pointer: " << globalPointer << std::endl;
    std::cout << "Text size: " << originalText.size() << std::endl;

    while (globalPointer < (int)originalText.size() - k) // break when reaches the last kString
    {
        if (match != "")
        {
            std::cout << "Copy modelling..."<< std::endl;
            std::cout << "Index of Global Pointer before CopyModel: " << globalPointer << std::endl;
            copyModel();
            std::cout << "Index of Global Pointer after CopyModel: " << globalPointer << std::endl;
            std::cout << " Done." << std::endl;
        }
        else
        {
            std::cout << "Fallback mode...";
            std::cout << "Index of Global Pointer before fallback: " << globalPointer << std::endl;
            fallbackModel();
            std::cout << "Index of Global Pointer after fallback: " << globalPointer << std::endl;
            std::cout << " Done." << std::endl;
        }
        std::cout << "Finding next window...";
        match = findCopyModel();
        std::cout << " Done." << std::endl;
    }

    std::cout << "Total number of bits: " << (double)totalNumberOfBits << std::endl;
    std::cout << "Average number of bits per symbol: " << (double)(totalNumberOfBits / originalText.size()) << std::endl;
    std::cout << "Compression ratio: " << (double)originalText.size() / totalNumberOfBits << std::endl;
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
    while (prediction < threshold)
    {
        if (originalText[copyPointer] == originalText[globalPointer])
        {
            stats.incrementHits();
            this->totalNumberOfBits += -std::log2(prediction);
        }
        else
        {
            stats.incrementMisses();
            double comp_prediction = 1 - prediction;
            this->totalNumberOfBits += -std::log2(comp_prediction / (alphabetSize - 1));
        }

        incrementGlobalPointer();
        copyPointer++;
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
        // fallbackWindowSize = globalPointer;
        currentKString = originalText.substr(globalPointer, k);
        for (char c : currentKString)
        {
            prob = 1 / (double)alphabetSize;
            std::cout << "1 number of bits before: " << totalNumberOfBits << std::endl;
            this->totalNumberOfBits += -std::log2(prob);
            std::cout << "1 number of bits after: " << totalNumberOfBits << std::endl;
            incrementGlobalPointer();
        }
        
        incrementGlobalPointer();
    }
    else
    {
        std::string fallbackString = originalText.substr(globalPointer - fallbackWindowSize, fallbackWindowSize);
        currentKString = originalText.substr(globalPointer, k);

        std::map<char, int> charCounts;
        std::map<char, double> charProbabilities;

        for (char c : fallbackString)
        {
            charCounts[c]++;
        }
        for (const auto &pair : charCounts)
        {
            charProbabilities[pair.first] = (double)pair.second / (double)fallbackWindowSize;
        }


        // currently counts the whole kstring, but might be better to only a part of it
        for (char c : currentKString)
        {
            prob = charProbabilities[c];
            std::cout << "2 number of bits before: " << totalNumberOfBits << std::endl;
            this->totalNumberOfBits += -prob * std::log2(prob);
            std::cout << "2 number of bits after: " << totalNumberOfBits << std::endl;
            incrementGlobalPointer();
        }
        // exit fallback model and look for match again
    }
}

void CopyModel::incrementGlobalPointer()
{
    globalPointer++;
    predictedText += originalText[globalPointer];
    pastKStrings.push_back(currentKString);
}