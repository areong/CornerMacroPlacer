CC = g++ -std=c++11
INC = -I ./../
HEADERS_FLOORPLANVIEW = \
	./../view/FloorplanView.h\
	./../view/View.h\
	./../view/Window.h

HEADERS_FLOORPLANWINDOW = \
	./../view/FloorplanView.h\
	./../view/FloorplanWindow.h\
	./../view/TilePlaneView.h\
	./../view/View.h\
	./../view/Window.h

HEADERS_TILEPLANEVIEW = \
	./../cornerstitching/Corner.h\
	./../cornerstitching/Tile.h\
	./../cornerstitching/TilePlane.h\
	./../view/TilePlaneView.h\
	./../view/View.h\
	./../view/Window.h

HEADERS_VIEW = \
	./../view/View.h

HEADERS_WINDOW = \
	./../view/Window.h

all:
	make FloorplanView.o
	make FloorplanWindow.o
	make TilePlaneView.o
	make View.o
	make Window.o
FloorplanView.o: FloorplanView.cpp ${HEADERS_FLOORPLANVIEW}
	${CC} -c FloorplanView.cpp ${INC}
FloorplanWindow.o: FloorplanWindow.cpp ${HEADERS_FLOORPLANWINDOW}
	${CC} -c FloorplanWindow.cpp ${INC}
TilePlaneView.o: TilePlaneView.cpp ${HEADERS_TILEPLANEVIEW}
	${CC} -c TilePlaneView.cpp ${INC}
View.o: View.cpp ${HEADERS_VIEW}
	${CC} -c View.cpp ${INC}
Window.o: Window.cpp ${HEADERS_WINDOW}
	${CC} -c Window.cpp ${INC}
clean: 
	rm *.o
