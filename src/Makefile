CC = g++ -std=c++11
INC = -I ./
HEADERS_MAIN = \
	./cornerstitching/TilePlane_test.h\
	./quadtree/Quadtree_test.h\
	./utils/Utils.h

OBJECTS = \
	./*.o\
	./cornerstitching/*.o\
	./quadtree/*.o\
	./utils/*.o\
	./view/*.o

LIBS = -lGL -lglut

all:
	cd cornerstitching/; make all
	cd quadtree/; make all
	cd utils/; make all
	cd view/; make all
	make main.o
	make placer
placer: ${OBJECTS}
	${CC} ${OBJECTS} ${LIBS} -o placer
main.o: main.cpp ${HEADERS_MAIN}
	${CC} -c main.cpp ${INC}
clean:
	cd cornerstitching/; make clean
	cd quadtree/; make clean
	cd utils/; make clean
	cd view/; make clean
	rm *.o placer
