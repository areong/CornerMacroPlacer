CC = g++ -std=c++11
INC = -I ./../
HEADERS_MACRO = \
	./../floorplan/Macro.h

HEADERS_SORTEDMACROS = \
	./../floorplan/Macro.h\
	./../floorplan/SortedMacros.h

all:
	make Macro.o
	make SortedMacros.o
Macro.o: Macro.cpp ${HEADERS_MACRO}
	${CC} -c Macro.cpp ${INC}
SortedMacros.o: SortedMacros.cpp ${HEADERS_SORTEDMACROS}
	${CC} -c SortedMacros.cpp ${INC}
clean: 
	rm *.o
