CC = g++ -std=c++11
INC = -I ./../
HEADERS_UTILS = \
	./../utils/Utils.h

all:
	make Utils.o
Utils.o: Utils.cpp ${HEADERS_UTILS}
	${CC} -c Utils.cpp ${INC}
clean: 
	rm *.o
