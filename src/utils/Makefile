INC = -I ./../
HEADERS_MATRIX2D = \
	./../utils/Matrix2d.h

HEADERS_UTILS = \
	./../utils/Utils.h

all:
	make Matrix2d.o
	make Utils.o
Matrix2d.o: Matrix2d.cpp $(HEADERS_MATRIX2D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Matrix2d.cpp $(INC)
Utils.o: Utils.cpp $(HEADERS_UTILS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Utils.cpp $(INC)
clean: 
	rm *.o
