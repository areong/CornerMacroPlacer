CC = g++ -std=c++11
INC = -I ./../
HEADERS_MACRO = \
	./../floorplan/Macro.h

all:
	make Macro.o
Macro.o: Macro.cpp ${HEADERS_MACRO}
	${CC} -c Macro.cpp ${INC}
clean: 
	rm *.o
