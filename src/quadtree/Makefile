CC = g++ -std=c++11
INC = -I ./../
HEADERS_POSITION = \
	./../quadtree/Point.h\
	./../quadtree/Position.h

HEADERS_POSITIONVALUE = \
	./../quadtree/Point.h\
	./../quadtree/PointValue.h\
	./../quadtree/Position.h\
	./../quadtree/PositionValue.h

HEADERS_QUADTREE = \
	./../quadtree/Point.h\
	./../quadtree/PointValue.h\
	./../quadtree/Quadtree.h\
	./../utils/Utils.h

HEADERS_QUADTREE_TEST = \
	./../quadtree/Point.h\
	./../quadtree/PointValue.h\
	./../quadtree/Position.h\
	./../quadtree/PositionValue.h\
	./../quadtree/Quadtree.h\
	./../quadtree/Quadtree_test.h

all:
	make Position.o
	make PositionValue.o
	make Quadtree.o
	make Quadtree_test.o
Position.o: Position.cpp ${HEADERS_POSITION}
	${CC} -c Position.cpp ${INC}
PositionValue.o: PositionValue.cpp ${HEADERS_POSITIONVALUE}
	${CC} -c PositionValue.cpp ${INC}
Quadtree.o: Quadtree.cpp ${HEADERS_QUADTREE}
	${CC} -c Quadtree.cpp ${INC}
Quadtree_test.o: Quadtree_test.cpp ${HEADERS_QUADTREE_TEST}
	${CC} -c Quadtree_test.cpp ${INC}
clean: 
	rm *.o
