#include "cornersequence/CornerSequence_test.h"
#include "cornerstitching/TilePlane_test.h"
#include "floorplan/Floorplan_test.h"
#include "quadtree/Quadtree_test.h"
#include "sa/SimulatedAnnealing_test.h"
#include "utils/Utils.h"

int main(int argc, char **argv) {
    Utils::run_srand();
    //testTilePlane();
    //testQuadtree();
    //testCornerSequence();
    //testFloorplan();
    testSimulatedAnnealing(argc, argv);
    return 0;
}