#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

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
};

#endif