CC = g++ -std=c++11
INC = -I ./../
HEADERS_TILEPLANE = \
	./../cornerstitching/Tile.h\
	./../cornerstitching/TilePlane.h\
	./../cornerstitching/TraversalTaskCollectAllTiles.h\
	./../cornerstitching/TraversalTask.h

HEADERS_TILE = \
	./../cornerstitching/Tile.h

HEADERS_TILEPLANE_TEST = \
	./../cornerstitching/TilePlane_test.h\
	./../cornerstitching/TilePlane.h\
	./../view/FloorplanWindow.h\
	./../cornerstitching/HorizontalTilePlane.h\
	./../cornerstitching/Tile.h\
	./../view/Window.h

HEADERS_HORIZONTALTILEPLANE = \
	./../cornerstitching/HorizontalTilePlane.h\
	./../cornerstitching/Tile.h\
	./../cornerstitching/TilePlane.h

HEADERS_TRAVERSALTASKCOLLECTALLTILES = \
	./../cornerstitching/TraversalTaskCollectAllTiles.h\
	./../cornerstitching/TraversalTask.h

all:
	make TilePlane.o
	make Tile.o
	make TilePlane_test.o
	make HorizontalTilePlane.o
	make TraversalTaskCollectAllTiles.o
TilePlane.o: TilePlane.cpp ${HEADERS_TILEPLANE}
	${CC} -c TilePlane.cpp ${INC}
Tile.o: Tile.cpp ${HEADERS_TILE}
	${CC} -c Tile.cpp ${INC}
TilePlane_test.o: TilePlane_test.cpp ${HEADERS_TILEPLANE_TEST}
	${CC} -c TilePlane_test.cpp ${INC}
HorizontalTilePlane.o: HorizontalTilePlane.cpp ${HEADERS_HORIZONTALTILEPLANE}
	${CC} -c HorizontalTilePlane.cpp ${INC}
TraversalTaskCollectAllTiles.o: TraversalTaskCollectAllTiles.cpp ${HEADERS_TRAVERSALTASKCOLLECTALLTILES}
	${CC} -c TraversalTaskCollectAllTiles.cpp ${INC}
clean: 
	rm *.o
