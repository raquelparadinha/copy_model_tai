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
CopyModel::CopyModel(int k, double t, int a, std::string oT, std::unordered_map<std::string, std::vector<int>> kStringsPos, int size) : k(k), threshold(t), alpha(a), originalText(oT), kStringsPositions(kStringsPos), alphabetSize(size)
{
    this->pastKStrings = std::vector<std::string>();

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
    std::string match = findCopyModel();
    while (globalPointer < (int)originalText.size()) // break when file ends
    {
        if (match != "")
        {
            copyModel();
        }
        else
        {
            fallbackModel();
        }
        match = findCopyModel();
    }

    std::cout << "Total number of bits: " << totalNumberOfBits << std::endl;
    std::cout << "Average number of bits per symbol: " << totalNumberOfBits / originalText.size() << std::endl;
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

    // Iniializa as stats e o threshold
    Stats stats(alpha);

    // Compara o simbolo da posição atual com o simbolo da posição do copy model anterior
    double prediction = stats.getProbability();
    while (prediction < threshold)
    {
        if (originalText[copyPointer] == originalText[globalPointer])
        {
            stats.incrementHits();
            this->totalNumberOfBits += std::log2(prediction);
        }
        else
        {
            stats.incrementMisses();
            double comp_prediction = 1 - prediction;
            this->totalNumberOfBits += std::log2(comp_prediction / (alphabetSize - 1));
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
        return currentKString;
    }

    std::cout << "No copy found." << std::endl;

    return "";
}

void CopyModel::fallbackModel()
{
    int fallbackWindowSize = 200;
    std::string fallbackString = originalText.substr(globalPointer - fallbackWindowSize, globalPointer);
    currentKString = originalText.substr(globalPointer, k);

    std::map<char, int> charCounts;
    std::map<char, double> charProbabilities;

    for (char c : fallbackString)
    {
        charCounts[c]++;
    }
    for (const auto &pair : charCounts)
    {
        charProbabilities[pair.first] = pair.second / fallbackWindowSize;
    }

    // currently counts the whole kstring, but might be better to only a part of it
    for (char c : currentKString)
    {
        this->totalNumberOfBits += std::log2(charProbabilities[c]);
        incrementGlobalPointer();
    }
    // exit fallback model and look for match again
}

void CopyModel::incrementGlobalPointer()
{
    globalPointer++;
    predictedText += originalText[globalPointer];
    pastKStrings.push_back(currentKString);
}