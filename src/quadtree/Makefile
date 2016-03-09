CC = g++ -std=c++11
INC = -I ./../
HEADERS_POINT = \
	./../quadtree/Point.h

HEADERS_QUADTREE_TEST = \
	./../quadtree/Position.h\
	./../quadtree/Point.h\
	./../quadtree/Quadtree.h\
	./../quadtree/Quadtree_test.h

HEADERS_QUADTREE = \
	./../quadtree/Quadtree.h\
	./../quadtree/Point.h\
	./../utils/Utils.h

HEADERS_POSITION = \
	./../quadtree/Position.h\
	./../quadtree/Point.h

all:
	make Point.o
	make Quadtree_test.o
	make Quadtree.o
	make Position.o
Point.o: Point.cpp ${HEADERS_POINT}
	${CC} -c Point.cpp ${INC}
Quadtree_test.o: Quadtree_test.cpp ${HEADERS_QUADTREE_TEST}
	${CC} -c Quadtree_test.cpp ${INC}
Quadtree.o: Quadtree.cpp ${HEADERS_QUADTREE}
	${CC} -c Quadtree.cpp ${INC}
Position.o: Position.cpp ${HEADERS_POSITION}
	${CC} -c Position.cpp ${INC}
clean: 
	rm *.o
