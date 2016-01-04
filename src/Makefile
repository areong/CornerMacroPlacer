CC = g++ -std=c++11
INC = -I ./
HEADERS_MAIN = \
	./cornerstitching/TilePlane_test.h

OBJECTS = \
	./*.o\
	./view/*.o\
	./cornerstitching/*.o

LIBS = -lGL -lglut

all:
	cd view/; make all
	cd cornerstitching/; make all
	make main.o
	make placer
placer: ${OBJECTS}
	${CC} ${OBJECTS} ${LIBS} -o placer
main.o: main.cpp ${HEADERS_MAIN}
	${CC} -c main.cpp ${INC}
clean:
	cd view/; make clean
	cd cornerstitching/; make clean
	rm *.o placer
