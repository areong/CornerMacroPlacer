INC = -I ./../
HEADERS_FLOORPLAN = \
	./../floorplan/Floorplan.h

HEADERS_MACRO = \
	./../floorplan/Macro.h

HEADERS_SORTEDMACROS = \
	./../floorplan/Macro.h\
	./../floorplan/SortedMacros.h

all:
	make Floorplan.o
	make Macro.o
	make SortedMacros.o
Floorplan.o: Floorplan.cpp $(HEADERS_FLOORPLAN)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Floorplan.cpp $(INC)
Macro.o: Macro.cpp $(HEADERS_MACRO)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Macro.cpp $(INC)
SortedMacros.o: SortedMacros.cpp $(HEADERS_SORTEDMACROS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c SortedMacros.cpp $(INC)
clean: 
	rm *.o
