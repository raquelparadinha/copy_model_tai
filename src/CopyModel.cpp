#include "CopyModel.h"
#include "Reader.h"
#include "Stats.h"
#include <algorithm>
#include <cmath>
#include <map>
#include <chrono>

/**
 * @brief Constructs a CopyModel object.
 *
 * @param k The value of k.
 * @param t The value of threshold.
 * @param a The value of alpha.
 * @param oT The value of originalText.
 * @param kStringsPos The value of kStringsPositions.
 * @param alphabet The alphabet of the text.
 */
CopyModel::CopyModel(std::string oT, std::vector<char> alphabet, int k, double t, double a, int window) : originalText(oT), alphabet(alphabet), k(k), threshold(t), alpha(a), fallbackWindowSize(window)
{
    this->pastKStrings = {};
    this->alphabetSize = alphabet.size();

    this->globalPointer = 0;
    this->copyPointer = 0;

    this->currentKString = "";

    this->totalNumberOfBits = 0;
}

/**
 * Runs the CopyModel.
 *
 * corre esta coisa toda
 *
 */
void CopyModel::run()
{
    std::string match;

    auto start = std::chrono::high_resolution_clock::now();
    while (globalPointer < (int)originalText.size()) // break when reaches the last kString
    {
        match = findCopyModel();
        
        if (match != "")
        {
            //std::cout << "Copy model" << std::endl;
            copyModel();
        }
        else
        {
            //std::cout << "Fallback model" << std::endl;
            fallbackModel();
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << std::endl << "Done. Results:"<< std::endl;
    std::cout << "Parameters: k=" << k << ", threshold=" << threshold << ", alpha=" << alpha << ", fallbackWindowSize=" << fallbackWindowSize << std::endl;
    std::cout << "Execution time: " << duration.count() / 1000 << " seconds" << std::endl;
    std::cout << "Total number of bits: " << (double)totalNumberOfBits << std::endl;
    std::cout << "Average number of bits per symbol: " << (double)(totalNumberOfBits / originalText.size()) << std::endl;
    std::cout << "Compression ratio: " << ((double)originalText.size() * 8) / totalNumberOfBits << std::endl;
    std::cout << "Compression percentage: " << (1 - (totalNumberOfBits / (originalText.size() * 8))) * 100 << "%" << std::endl;
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
    if (globalPointer >= k)
    {
        if (pastKStrings.find(currentKString) != pastKStrings.end())
        {
            // Found a matching window for the past k characters
            copyPointer = pastKStrings[currentKString].front();

            return currentKString;
        }
    }

    return "";
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
    while (prediction > threshold && globalPointer < (int)originalText.size())
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

        prediction = stats.getProbability();
    }
    // Exit loop meaning the copy model accuracy is too low
}

void CopyModel::fallbackModel()
{
    double prob;
    int n = 1; // chars of the kstring to count on this fallback model

    if (globalPointer < fallbackWindowSize)
    {
        for (int i = 0; i < n; i++)
        {
            prob = 1 / (double)alphabetSize;
            this->totalNumberOfBits += -std::log2(prob);
            incrementGlobalPointer();
        }
    }
    else
    {
        std::string fallbackString = originalText.substr(globalPointer - fallbackWindowSize, fallbackWindowSize);

        std::map<char, int> charCounts;
        std::map<char, double> charProbabilities;
        for (char c : alphabet)
        {
            charCounts[c] = 0;
            charProbabilities[c] = 0;
        }

        for (char c : fallbackString)
        {
            charCounts[c]++;
        }

        for (std::pair pair : charCounts) {
            charProbabilities[pair.first] = (double)pair.second / fallbackWindowSize;

            prob += -(double)charProbabilities[pair.first] * (double)std::log2(charProbabilities[pair.first]);
        }

        this->totalNumberOfBits += prob;

        incrementGlobalPointer();
    }
}

void CopyModel::incrementGlobalPointer()
{
    if (globalPointer >= k - 1) {
        currentKString = originalText.substr(globalPointer - k + 1, k);
        pastKStrings[currentKString].push_back(globalPointer);
    }
    
    globalPointer++;
    
    // std::cout << globalPointer << " " << originalText.size() << std::endl;   
}

int CopyModel::getStats()
{
    return totalNumberOfBits;
    
    }