#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

#include <string>
#include <vector>

class Utils {
public:
    Utils();
    ~Utils();
    /*
    Call this method before calling any random methods.
    */
    static void run_srand();
    /*
    Return random double in [0, 1).
    */
    static double random();
    /*
    Assume end >= start.
    Return random integer in [start, end).
    */
    static int randint(int start, int end);
    /*
    Split a string to a vector of strings.
    PLEASE DELETE the returned vector.
    @return A vector of strings.
    @param source The string to split.
    @param splitter Split the string at characters matching one of the characters
                    in splitter.
    */
    static std::vector<std::string> *splitString(std::string source, std::string splitter);
};

#endif