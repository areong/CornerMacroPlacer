#ifndef CORNERSTITCHING_TILEPLANE_TEST_H_
#define CORNERSTITCHING_TILEPLANE_TEST_H_

class TilePlane;

void testTilePlane_memoryLeak();
void testTilePlane_CornerHorizontalTilePlane();
void testTilePlane_HorizontalTilePlane();
void testTilePlane_sortTiles();
void displayTilePlane(TilePlane *tilePlane, int xStart, int yStart, int xEnd, int yEnd);
void testTilePlane();

#endif