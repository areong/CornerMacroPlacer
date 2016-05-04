#include "utils/Utils.h"
#include <cstdlib>
#include <cstring>
#include <ctime>

Utils::Utils() {

}

Utils::~Utils() {

}

void Utils::run_srand() {
    srand(time(0));
}

double Utils::random() {
    return (double) rand() / ((double) RAND_MAX + 1);
}

int Utils::randint(int start, int end) {
    int range = end - start;
    return (int) ((double) rand() / ((double) RAND_MAX + 1) * range) + start;
}

std::vector<std::string> *Utils::splitString(std::string source, std::string splitter) {
    // http://www.cplusplus.com/reference/string/string/c_str/

    std::vector<std::string> *results = new std::vector<std::string>();
    char *charsSource = new char[source.length() + 1];
    char *charsSplitter = new char[splitter.length() + 1];
    std::strcpy(charsSource, source.c_str());
    std::strcpy(charsSplitter, splitter.c_str());
    char *token = std::strtok(charsSource, charsSplitter);
    while (token != 0) {
        results->push_back(std::string(token));
        token = std::strtok(NULL, charsSplitter);
    }
    delete[] charsSource;
    return results;
}