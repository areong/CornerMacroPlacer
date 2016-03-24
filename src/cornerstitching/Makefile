CC = g++ -std=c++11
INC = -I ./../
HEADERS_CORNER = \
	./../cornerstitching/Corner.h

HEADERS_CORNERHORIZONTALTILEPLANE = \
	./../cornerstitching/Corner.h\
	./../cornerstitching/CornerHorizontalTilePlane.h\
	./../cornerstitching/CornerVerticalTilePlane.h\
	./../cornerstitching/HorizontalTilePlane.h\
	./../cornerstitching/Tile.h\
	./../cornerstitching/TilePlane.h\
	./../cornerstitching/VerticalTilePlane.h

HEADERS_CORNERVERTICALTILEPLANE = \
	./../cornerstitching/Corner.h\
	./../cornerstitching/CornerHorizontalTilePlane.h\
	./../cornerstitching/CornerVerticalTilePlane.h\
	./../cornerstitching/HorizontalTilePlane.h\
	./../cornerstitching/Tile.h\
	./../cornerstitching/TilePlane.h\
	./../cornerstitching/VerticalTilePlane.h

HEADERS_HORIZONTALTILEPLANE = \
	./../cornerstitching/HorizontalTilePlane.h\
	./../cornerstitching/Tile.h\
	./../cornerstitching/TilePlane.h

HEADERS_TILE = \
	./../cornerstitching/Corner.h\
	./../cornerstitching/Tile.h

HEADERS_TILEPLANE = \
	./../cornerstitching/Tile.h\
	./../cornerstitching/TilePlane.h\
	./../cornerstitching/TraversalTask.h\
	./../cornerstitching/TraversalTaskCollectAllTiles.h

HEADERS_TILEPLANE_TEST = \
	./../cornerstitching/Corner.h\
	./../cornerstitching/CornerHorizontalTilePlane.h\
	./../cornerstitching/CornerVerticalTilePlane.h\
	./../cornerstitching/HorizontalTilePlane.h\
	./../cornerstitching/Tile.h\
	./../cornerstitching/TilePlane.h\
	./../cornerstitching/TilePlane_test.h\
	./../cornerstitching/VerticalTilePlane.h\
	./../view/FloorplanWindow.h\
	./../view/Window.h

HEADERS_TRAVERSALTASKCOLLECTALLTILES = \
	./../cornerstitching/TraversalTask.h\
	./../cornerstitching/TraversalTaskCollectAllTiles.h

HEADERS_VERTICALTILEPLANE = \
	./../cornerstitching/Tile.h\
	./../cornerstitching/TilePlane.h\
	./../cornerstitching/VerticalTilePlane.h

all:
	make Corner.o
	make CornerHorizontalTilePlane.o
	make CornerVerticalTilePlane.o
	make HorizontalTilePlane.o
	make Tile.o
	make TilePlane.o
	make TilePlane_test.o
	make TraversalTaskCollectAllTiles.o
	make VerticalTilePlane.o
Corner.o: Corner.cpp ${HEADERS_CORNER}
	${CC} -c Corner.cpp ${INC}
CornerHorizontalTilePlane.o: CornerHorizontalTilePlane.cpp ${HEADERS_CORNERHORIZONTALTILEPLANE}
	${CC} -c CornerHorizontalTilePlane.cpp ${INC}
CornerVerticalTilePlane.o: CornerVerticalTilePlane.cpp ${HEADERS_CORNERVERTICALTILEPLANE}
	${CC} -c CornerVerticalTilePlane.cpp ${INC}
HorizontalTilePlane.o: HorizontalTilePlane.cpp ${HEADERS_HORIZONTALTILEPLANE}
	${CC} -c HorizontalTilePlane.cpp ${INC}
Tile.o: Tile.cpp ${HEADERS_TILE}
	${CC} -c Tile.cpp ${INC}
TilePlane.o: TilePlane.cpp ${HEADERS_TILEPLANE}
	${CC} -c TilePlane.cpp ${INC}
TilePlane_test.o: TilePlane_test.cpp ${HEADERS_TILEPLANE_TEST}
	${CC} -c TilePlane_test.cpp ${INC}
TraversalTaskCollectAllTiles.o: TraversalTaskCollectAllTiles.cpp ${HEADERS_TRAVERSALTASKCOLLECTALLTILES}
	${CC} -c TraversalTaskCollectAllTiles.cpp ${INC}
VerticalTilePlane.o: VerticalTilePlane.cpp ${HEADERS_VERTICALTILEPLANE}
	${CC} -c VerticalTilePlane.cpp ${INC}
clean: 
	rm *.o
