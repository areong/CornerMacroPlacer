#ifndef SA_SIMULATEDANNEALING_TEST_H_
#define SA_SIMULATEDANNEALING_TEST_H_

class Floorplan;

Floorplan *testSimulatedAnnealing_createFloorplan();
void testSimulatedAnnealing_annealCornerSequence(int argc, char **argv);
void testSimulatedAnnealing(int argc, char **argv);

#endif