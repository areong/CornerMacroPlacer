INC = -I ./../
HEADERS_FLOORPLANVIEW = \
	./../floorplan/Floorplan.h\
	./../floorplan/Macro.h\
	./../floorplan/Module.h\
	./../view/FloorplanView.h\
	./../view/View.h\
	./../view/Window.h

HEADERS_FLOORPLANWINDOW = \
	./../floorplan/Floorplan.h\
	./../view/FloorplanView.h\
	./../view/FloorplanWindow.h\
	./../view/TilePlaneView.h\
	./../view/View.h\
	./../view/Window.h

HEADERS_TILEPLANEVIEW = \
	./../cornerstitching/Corner.h\
	./../cornerstitching/Tile.h\
	./../cornerstitching/TilePlane.h\
	./../quadtree/Point.h\
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
FloorplanView.o: FloorplanView.cpp $(HEADERS_FLOORPLANVIEW)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c FloorplanView.cpp $(INC)
FloorplanWindow.o: FloorplanWindow.cpp $(HEADERS_FLOORPLANWINDOW)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c FloorplanWindow.cpp $(INC)
TilePlaneView.o: TilePlaneView.cpp $(HEADERS_TILEPLANEVIEW)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c TilePlaneView.cpp $(INC)
View.o: View.cpp $(HEADERS_VIEW)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c View.cpp $(INC)
Window.o: Window.cpp $(HEADERS_WINDOW)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Window.cpp $(INC)
clean: 
	rm *.o
