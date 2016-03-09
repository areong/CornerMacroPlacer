CC = g++ -std=c++11
INC = -I ./../
HEADERS_FLOORPLANWINDOW = \
	./../view/Window.h\
	./../view/TilePlaneView.h\
	./../view/FloorplanWindow.h\
	./../view/View.h\
	./../view/FloorplanView.h

HEADERS_TILEPLANEVIEW = \
	./../view/Window.h\
	./../cornerstitching/Corner.h\
	./../cornerstitching/Tile.h\
	./../view/TilePlaneView.h\
	./../cornerstitching/TilePlane.h\
	./../view/View.h

HEADERS_VIEW = \
	./../view/View.h

HEADERS_FLOORPLANVIEW = \
	./../view/Window.h\
	./../view/FloorplanView.h\
	./../view/View.h

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
