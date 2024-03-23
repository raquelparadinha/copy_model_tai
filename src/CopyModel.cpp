#include "CopyModel.h"
#include "Reader.h"
#include <algorithm>

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
CopyModel::CopyModel(int k, double t, int a, std::string oT, std::unordered_map<std::string, std::vector<int>> kStringsPos, int size) :
    k(k), threshold(t), alpha(a), originalText(oT), kStringsPositions(kStringsPos), alphabetSize(size) {
        this->globalPointer = 0;
        this->localPointer = 0;
        this->predictedText = "";
        this->pastKStrings = std::vector<std::string>();
        this->currentKString = "";
}

/**
 * Finds the copy model.
 *
 * This function searches for repeated kStrings in the predicted text to start a Copy Model run.
 *
 * @return The first repeated kString found or an empty string if none is found.
 */
std::string CopyModel::findCopyModel() {
    currentKString = originalText.substr(globalPointer, k);
    pastKStrings.push_back(currentKString);
    predictedText += currentKString;

    incrementGlobalPointer();

    while (globalPointer < originalText.size()) {
        currentKString = originalText.substr(globalPointer, k);

        if (std::find(pastKStrings.begin(), pastKStrings.end(), currentKString) != pastKStrings.end()) {
            // Found a copy
            predictedText += originalText[globalPointer + k - 1];
            incrementGlobalPointer();
            return currentKString;
        } 

        pastKStrings.push_back(currentKString);
        predictedText += originalText[globalPointer + k - 1];
        incrementGlobalPointer();
    }

    std::cout << "No copy found." << std::endl;

    return "";
}

void runCopyModel() {
    

}

void CopyModel::incrementGlobalPointer() {
    this->globalPointer++;
}