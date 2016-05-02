INC = -I ./../
HEADERS_CORNERSEQUENCE = \
	./../cornersequence/CornerSequence.h\
	./../cornerstitching/Corner.h\
	./../cornerstitching/CornerHorizontalTilePlane.h\
	./../cornerstitching/CornerPosition.h\
	./../cornerstitching/CornerSize.h\
	./../cornerstitching/CornerVerticalTilePlane.h\
	./../cornerstitching/HorizontalTilePlane.h\
	./../cornerstitching/Tile.h\
	./../cornerstitching/TilePlane.h\
	./../cornerstitching/VerticalTilePlane.h\
	./../floorplan/Macro.h\
	./../floorplan/SortedMacros.h\
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
	./../floorplan/SortedMacros.h\
	./../quadtree/Point.h\
	./../utils/Utils.h\
	./../view/FloorplanWindow.h\
	./../view/Window.h

all:
	make CornerSequence.o
	make CornerSequence_test.o
CornerSequence.o: CornerSequence.cpp $(HEADERS_CORNERSEQUENCE)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c CornerSequence.cpp $(INC)
CornerSequence_test.o: CornerSequence_test.cpp $(HEADERS_CORNERSEQUENCE_TEST)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c CornerSequence_test.cpp $(INC)
clean: 
	rm *.o
