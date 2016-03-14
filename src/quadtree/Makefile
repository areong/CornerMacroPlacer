CC = g++ -std=c++11
INC = -I ./../
HEADERS_POINT = \
	./../quadtree/Point.h

HEADERS_POSITION = \
	./../quadtree/Point.h\
	./../quadtree/Position.h

HEADERS_QUADTREE = \
	./../quadtree/Point.h\
	./../quadtree/Quadtree.h\
	./../utils/Utils.h

HEADERS_QUADTREE_TEST = \
	./../quadtree/Point.h\
	./../quadtree/Position.h\
	./../quadtree/Quadtree.h\
	./../quadtree/Quadtree_test.h

all:
	make Point.o
	make Position.o
	make Quadtree.o
	make Quadtree_test.o
Point.o: Point.cpp ${HEADERS_POINT}
	${CC} -c Point.cpp ${INC}
Position.o: Position.cpp ${HEADERS_POSITION}
	${CC} -c Position.cpp ${INC}
Quadtree.o: Quadtree.cpp ${HEADERS_QUADTREE}
	${CC} -c Quadtree.cpp ${INC}
Quadtree_test.o: Quadtree_test.cpp ${HEADERS_QUADTREE_TEST}
	${CC} -c Quadtree_test.cpp ${INC}
clean: 
	rm *.o
