CXXFLAGS=-g -std=c++11
CXX=g++

iosched: simulation.cpp io_request.hpp scheduler.hpp utils.cpp utils.hpp
	$(CXX) $(CXXFLAGS) simulation.cpp utils.cpp -o iosched

clean:
	rm -rf *.o
	rm -f iosched