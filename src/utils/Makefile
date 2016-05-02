INC = -I ./../
HEADERS_UTILS = \
	./../utils/Utils.h

all:
	make Utils.o
Utils.o: Utils.cpp $(HEADERS_UTILS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Utils.cpp $(INC)
clean: 
	rm *.o
