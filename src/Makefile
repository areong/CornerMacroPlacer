CC = g++ -std=c++11
INC = -I ./
HEADERS_MAIN = \
	./cornerstitching/TilePlane_test.h\
	./quadtree/Quadtree_test.h\
	./utils/Utils.h

OBJECTS = \
	./*.o\
	./view/*.o\
	./utils/*.o\
	./cornerstitching/*.o\
	./quadtree/*.o

LIBS = -lGL -lglut

all:
	cd view/; make all
	cd utils/; make all
	cd cornerstitching/; make all
	cd quadtree/; make all
	make main.o
	make placer
placer: ${OBJECTS}
	${CC} ${OBJECTS} ${LIBS} -o placer
main.o: main.cpp ${HEADERS_MAIN}
	${CC} -c main.cpp ${INC}
clean:
	cd view/; make clean
	cd utils/; make clean
	cd cornerstitching/; make clean
	cd quadtree/; make clean
	rm *.o placer
