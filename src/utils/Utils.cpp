#include "utils/Utils.h"
#include <cstdlib>
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