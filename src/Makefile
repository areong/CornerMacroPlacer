CXXFLAGS = -std=c++11
INC = -I ./
HEADERS_MAIN = \
	./cornersequence/CornerSequence_test.h\
	./cornerstitching/TilePlane_test.h\
	./floorplan/Floorplan_test.h\
	./quadtree/Quadtree_test.h\
	./sa/SimulatedAnnealing_test.h\
	./utils/Utils.h

OBJECTS = \
	./*.o\
	./cornersequence/*.o\
	./cornerstitching/*.o\
	./floorplan/*.o\
	./quadtree/*.o\
	./sa/*.o\
	./utils/*.o\
	./view/*.o

LDLIBS = -lGL -lglut

all:
	cd cornersequence/; make all
	cd cornerstitching/; make all
	cd floorplan/; make all
	cd quadtree/; make all
	cd sa/; make all
	cd utils/; make all
	cd view/; make all
	make main.o
	make placer
placer: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o placer
main.o: main.cpp $(HEADERS_MAIN)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c main.cpp $(INC)
clean:
	cd cornersequence/; make clean
	cd cornerstitching/; make clean
	cd floorplan/; make clean
	cd quadtree/; make clean
	cd sa/; make clean
	cd utils/; make clean
	cd view/; make clean
	rm *.o placer
