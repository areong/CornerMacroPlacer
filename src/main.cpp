#include "cornerstitching/TilePlane_test.h"
#include "quadtree/Quadtree_test.h"
#include "utils/Utils.h"

int main() {
    Utils::run_srand();
    testTilePlane();
    //testQuadtree();
    return 0;
}