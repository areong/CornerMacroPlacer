CC = g++ -std=c++11
INC = -I ./../
HEADERS_TILEPLANE = \
	./../cornerstitching/TilePlane.h\
	./../cornerstitching/TraversalTaskCollectAllTiles.h\
	./../cornerstitching/Tile.h\
	./../cornerstitching/TraversalTask.h

HEADERS_TILE = \
	./../cornerstitching/Corner.h\
	./../cornerstitching/Tile.h

HEADERS_TILEPLANE_TEST = \
	./../cornerstitching/HorizontalTilePlane.h\
	./../cornerstitching/CornerHorizontalTilePlane.h\
	./../cornerstitching/Tile.h\
	./../cornerstitching/TilePlane.h\
	./../view/FloorplanWindow.h\
	./../cornerstitching/TilePlane_test.h\
	./../view/Window.h

HEADERS_HORIZONTALTILEPLANE = \
	./../cornerstitching/HorizontalTilePlane.h\
	./../cornerstitching/TilePlane.h\
	./../cornerstitching/Tile.h

HEADERS_CORNER = \
	./../cornerstitching/Corner.h

HEADERS_TRAVERSALTASKCOLLECTALLTILES = \
	./../cornerstitching/TraversalTaskCollectAllTiles.h\
	./../cornerstitching/TraversalTask.h

HEADERS_CORNERHORIZONTALTILEPLANE = \
	./../cornerstitching/HorizontalTilePlane.h\
	./../cornerstitching/TilePlane.h\
	./../cornerstitching/CornerHorizontalTilePlane.h\
	./../cornerstitching/Corner.h\
	./../cornerstitching/Tile.h

all:
	make TilePlane.o
	make Tile.o
	make TilePlane_test.o
	make HorizontalTilePlane.o
	make Corner.o
	make TraversalTaskCollectAllTiles.o
	make CornerHorizontalTilePlane.o
TilePlane.o: TilePlane.cpp ${HEADERS_TILEPLANE}
	${CC} -c TilePlane.cpp ${INC}
Tile.o: Tile.cpp ${HEADERS_TILE}
	${CC} -c Tile.cpp ${INC}
TilePlane_test.o: TilePlane_test.cpp ${HEADERS_TILEPLANE_TEST}
	${CC} -c TilePlane_test.cpp ${INC}
HorizontalTilePlane.o: HorizontalTilePlane.cpp ${HEADERS_HORIZONTALTILEPLANE}
	${CC} -c HorizontalTilePlane.cpp ${INC}
Corner.o: Corner.cpp ${HEADERS_CORNER}
	${CC} -c Corner.cpp ${INC}
TraversalTaskCollectAllTiles.o: TraversalTaskCollectAllTiles.cpp ${HEADERS_TRAVERSALTASKCOLLECTALLTILES}
	${CC} -c TraversalTaskCollectAllTiles.cpp ${INC}
CornerHorizontalTilePlane.o: CornerHorizontalTilePlane.cpp ${HEADERS_CORNERHORIZONTALTILEPLANE}
	${CC} -c CornerHorizontalTilePlane.cpp ${INC}
clean: 
	rm *.o
