CC = g++ -std=c++11
INC = -I ./../
HEADERS_FLOORPLANWINDOW = \
	./../view/TilePlaneView.h\
	./../view/FloorplanWindow.h\
	./../view/FloorplanView.h\
	./../view/View.h\
	./../view/Window.h

HEADERS_TILEPLANEVIEW = \
	./../cornerstitching/TilePlane.h\
	./../view/TilePlaneView.h\
	./../cornerstitching/Tile.h\
	./../view/View.h\
	./../cornerstitching/TraversalTaskCollectAllTiles.h\
	./../view/Window.h\
	./../cornerstitching/TraversalTask.h

HEADERS_VIEW = \
	./../view/View.h

HEADERS_FLOORPLANVIEW = \
	./../view/FloorplanView.h\
	./../view/View.h\
	./../view/Window.h

HEADERS_WINDOW = \
	./../view/Window.h

all:
	make FloorplanWindow.o
	make TilePlaneView.o
	make View.o
	make FloorplanView.o
	make Window.o
FloorplanWindow.o: FloorplanWindow.cpp ${HEADERS_FLOORPLANWINDOW}
	${CC} -c FloorplanWindow.cpp ${INC}
TilePlaneView.o: TilePlaneView.cpp ${HEADERS_TILEPLANEVIEW}
	${CC} -c TilePlaneView.cpp ${INC}
View.o: View.cpp ${HEADERS_VIEW}
	${CC} -c View.cpp ${INC}
FloorplanView.o: FloorplanView.cpp ${HEADERS_FLOORPLANVIEW}
	${CC} -c FloorplanView.cpp ${INC}
Window.o: Window.cpp ${HEADERS_WINDOW}
	${CC} -c Window.cpp ${INC}
clean: 
	rm *.o
