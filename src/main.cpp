#include "cornersequence/CornerSequence_test.h"
#include "cornerstitching/TilePlane_test.h"
#include "quadtree/Quadtree_test.h"
#include "utils/Utils.h"

int main() {
    Utils::run_srand();
    //testTilePlane();
    //testQuadtree();
    testCornerSequence();
    return 0;
}