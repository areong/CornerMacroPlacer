CC = g++ -std=c++11
INC = -I ./../
HEADERS_CORNERSEQUENCE = \
	./../cornersequence/CornerSequence.h\
	./../cornerstitching/Corner.h\
	./../cornerstitching/CornerHorizontalTilePlane.h\
	./../cornerstitching/CornerSize.h\
	./../cornerstitching/CornerVerticalTilePlane.h\
	./../cornerstitching/HorizontalTilePlane.h\
	./../cornerstitching/Tile.h\
	./../cornerstitching/TilePlane.h\
	./../cornerstitching/VerticalTilePlane.h\
	./../floorplan/Macro.h\
	./../quadtree/Point.h\
	./../quadtree/PointValue.h\
	./../quadtree/Quadtree.h

HEADERS_CORNERSEQUENCE_TEST = \
	./../cornersequence/CornerSequence.h\
	./../cornersequence/CornerSequence_test.h\
	./../cornerstitching/Corner.h\
	./../cornerstitching/CornerHorizontalTilePlane.h\
	./../cornerstitching/CornerVerticalTilePlane.h\
	./../cornerstitching/HorizontalTilePlane.h\
	./../cornerstitching/Tile.h\
	./../cornerstitching/TilePlane.h\
	./../cornerstitching/VerticalTilePlane.h\
	./../floorplan/Macro.h\
	./../quadtree/Point.h\
	./../utils/Utils.h\
	./../view/FloorplanWindow.h\
	./../view/Window.h

all:
	make CornerSequence.o
	make CornerSequence_test.o
CornerSequence.o: CornerSequence.cpp ${HEADERS_CORNERSEQUENCE}
	${CC} -c CornerSequence.cpp ${INC}
CornerSequence_test.o: CornerSequence_test.cpp ${HEADERS_CORNERSEQUENCE_TEST}
	${CC} -c CornerSequence_test.cpp ${INC}
clean: 
	rm *.o
