INC = -I ./../
HEADERS_CELL = \
	./../floorplan/Cell.h\
	./../floorplan/Module.h

HEADERS_FLOORPLAN = \
	./../floorplan/Cell.h\
	./../floorplan/Floorplan.h\
	./../floorplan/Macro.h\
	./../floorplan/Module.h\
	./../floorplan/Net.h\
	./../floorplan/Pin.h\
	./../floorplan/Rectangle.h\
	./../floorplan/Terminal.h\
	./../utils/Matrix2d.h\
	./../utils/Utils.h

HEADERS_FLOORPLAN_TEST = \
	./../floorplan/Floorplan.h\
	./../floorplan/Floorplan_test.h\
	./../floorplan/Macro.h\
	./../floorplan/Module.h

HEADERS_MACRO = \
	./../floorplan/Macro.h\
	./../floorplan/Module.h

HEADERS_MODULE = \
	./../floorplan/Module.h\
	./../floorplan/Pin.h\
	./../floorplan/Rectangle.h

HEADERS_NET = \
	./../floorplan/Floorplan.h\
	./../floorplan/Net.h\
	./../floorplan/Pin.h

HEADERS_PIN = \
	./../floorplan/Pin.h

HEADERS_RECTANGLE = \
	./../floorplan/Rectangle.h

HEADERS_SORTEDMACROS = \
	./../floorplan/Macro.h\
	./../floorplan/Module.h\
	./../floorplan/SortedMacros.h

HEADERS_TERMINAL = \
	./../floorplan/Module.h\
	./../floorplan/Pin.h\
	./../floorplan/Terminal.h

all:
	make Cell.o
	make Floorplan.o
	make Floorplan_test.o
	make Macro.o
	make Module.o
	make Net.o
	make Pin.o
	make Rectangle.o
	make SortedMacros.o
	make Terminal.o
Cell.o: Cell.cpp $(HEADERS_CELL)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Cell.cpp $(INC)
Floorplan.o: Floorplan.cpp $(HEADERS_FLOORPLAN)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Floorplan.cpp $(INC)
Floorplan_test.o: Floorplan_test.cpp $(HEADERS_FLOORPLAN_TEST)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Floorplan_test.cpp $(INC)
Macro.o: Macro.cpp $(HEADERS_MACRO)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Macro.cpp $(INC)
Module.o: Module.cpp $(HEADERS_MODULE)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Module.cpp $(INC)
Net.o: Net.cpp $(HEADERS_NET)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Net.cpp $(INC)
Pin.o: Pin.cpp $(HEADERS_PIN)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Pin.cpp $(INC)
Rectangle.o: Rectangle.cpp $(HEADERS_RECTANGLE)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Rectangle.cpp $(INC)
SortedMacros.o: SortedMacros.cpp $(HEADERS_SORTEDMACROS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c SortedMacros.cpp $(INC)
Terminal.o: Terminal.cpp $(HEADERS_TERMINAL)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Terminal.cpp $(INC)
clean: 
	rm *.o
