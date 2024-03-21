#include "CopyModel.h"
#include "Reader.h"

CopyModel::CopyModel(int k, double threshold, int alpha, int alphabet_size, std::string file) : k(k), threshold(threshold), alpha(alpha), alphabet_size(alphabet_size) {
    this->reader = Reader(file);
}

void runCopyModel() {
    

}