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
 * @param alphabet The alphabet of the text.
 */
CopyModel::CopyModel(std::string oT, std::vector<char> alphabet, std::unordered_map<std::string, std::vector<int>> kStringsPos, int k, double t, int a, int window) : originalText(oT), alphabet(alphabet), kStringsPositions(kStringsPos), k(k), threshold(t), alpha(a), fallbackWindowSize(window)
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
 * corre esta coisa toda
 *
 */
void CopyModel::run()
{
    std::cout << "Text size: " << originalText.size() << std::endl;
    std::cout << "kString size: " << k << std::endl;
    std::cout << "Threshold: " << threshold << std::endl;
    std::cout << "Alpha: " << alpha << std::endl;
    std::cout << "FallbackWindow size: " << fallbackWindowSize << std::endl;

    std::string match = findCopyModel();

    while (globalPointer < (int)originalText.size() - k) // break when reaches the last kString
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

    std::cout << std::endl << "Done. Results:"<< std::endl;
    std::cout << "Total number of bits: " << (double)totalNumberOfBits << std::endl;
    std::cout << "Average number of bits per symbol: " << (double)(totalNumberOfBits / originalText.size()) << std::endl;
    std::cout << "Compression ratio: " << (double)originalText.size() / totalNumberOfBits << std::endl;
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

        // std::cout << "Hits: " << stats.getNumberOfHits() << ", " << "Misses: " << stats.getNumberOfMisses() << std::endl;

        incrementGlobalPointer();
        copyPointer++;

        prediction = stats.getProbability();
    }
    // Exit loop meaning the copy model accuracy is too low
}

void CopyModel::fallbackModel()
{
    double prob;
    int n = 1;
    if (globalPointer < fallbackWindowSize)
    {
        currentKString = originalText.substr(globalPointer, k);
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
        currentKString = originalText.substr(globalPointer, k);

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

        for (char c : alphabet)
        {
            charProbabilities[c] = (double)(charCounts[c] + alpha) / (double)(fallbackWindowSize + alphabetSize * alpha);
        }

        for (char c : currentKString.substr(0, n))
        {
            prob = charProbabilities[c];
            this->totalNumberOfBits += -std::log2(prob);
            incrementGlobalPointer();
        }
    }
}

void CopyModel::incrementGlobalPointer()
{
    globalPointer++;
    predictedText += originalText[globalPointer];
    pastKStrings.push_back(currentKString);
}