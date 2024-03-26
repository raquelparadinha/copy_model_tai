#include "CopyModel.h"
#include "Reader.h"
#include "Stats.h"
#include <algorithm>
#include <cmath>
#include <map>
#include <chrono>
#include <thread>
#include <mutex>


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

std::map<int, std::vector<double>> rankings = {};
std::mutex mtx;
std::vector<std::thread> threads;



void insertIntoRanking(int pointer, std::vector<double> values) {
    std::lock_guard<std::mutex> lock(mtx); // Lock the mutex to synchronize access to the global map
    rankings[pointer] = values;
    //std::cout << "Inserted" << std::endl;
}

void CopyModel::startThread(int pointer, int local_pointer) {
    threads.push_back(std::thread([this, pointer, local_pointer]() {
        checkKString(pointer, local_pointer);
    }));
}


void CopyModel::checkKString(int pointer, int local_pointer) {

    Stats stats(alpha);

    int initial_pointer = pointer;

    double i = 0.0;
        double prediction = stats.getProbability();
        double information = 0.0;
        while (prediction > threshold && pointer < (int)originalText.size())
        {
            if (originalText[pointer] == originalText[local_pointer])
            {
                stats.incrementHits();
                //this->totalNumberOfBits += -std::log2(prediction);
                information += -std::log2(prediction);
            }
            else
            {
                stats.incrementMisses();
                double comp_prediction = 1 - prediction;
                //this->totalNumberOfBits += -std::log2(comp_prediction / (alphabetSize - 1));
                information += -std::log2(comp_prediction / (alphabetSize - 1));

            }

            local_pointer++;
            pointer++;

            prediction = stats.getProbability();
            i++;
        }
        //rankings[pointer] = {i,information};

    insertIntoRanking(initial_pointer, {i,information});
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
            std::cout << "Copy model" << std::endl;
            copyModel();
        }
        else
        {
            std::cout << "Fallback model" << std::endl;
            fallbackModel();
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << std::endl << "Done. Results:"<< std::endl;
    std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
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

    // Compara o simbolo da posição atual com o simbolo da posição do copy model anterior
    std::vector<int> pointers = pastKStrings[currentKString];

    int local_pointer = globalPointer;
    /*for (int pointer : pointers) {
        double i = 0.0;
        double prediction = stats.getProbability();
        double information = 0.0;
        while (prediction > threshold && local_pointer < (int)originalText.size())
        {
            if (originalText[pointer] == originalText[local_pointer])
            {
                stats.incrementHits();
                //this->totalNumberOfBits += -std::log2(prediction);
                information += -std::log2(prediction);
            }
            else
            {
                stats.incrementMisses();
                double comp_prediction = 1 - prediction;
                //this->totalNumberOfBits += -std::log2(comp_prediction / (alphabetSize - 1));
                information += -std::log2(comp_prediction / (alphabetSize - 1));

            }

            local_pointer++;
            pointer++;

            prediction = stats.getProbability();
            i++;
        }
        rankings[pointer] = {i,information};
    
    }*/

    rankings.clear();
    threads.clear();

    for (int pointer : pointers) {
        startThread(pointer, local_pointer);
    }

    // Join all threads
    for (auto& thread : threads) {
        thread.join();
    }

    int biggest = 0;
    int winner_pointer;
    int winner_information;

    for (const auto& pair : rankings) {

        std::vector<double> doubles = pair.second;

        if (doubles[0] > biggest) {
            biggest = doubles[0];
            winner_pointer = pair.first;
            winner_information = doubles[1];
        }
    }

    this->totalNumberOfBits += winner_information;
    this->totalNumberOfBits += -std::log2(rankings.size());

    std::cout << "Finished " << currentKString << std::endl;
    std::cout << "Best Pointer: " << winner_pointer << std::endl;


    globalPointer += biggest -1;
    incrementGlobalPointer();
    
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
    
    std::cout << globalPointer << " " << originalText.size() << std::endl;   
}