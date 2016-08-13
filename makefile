CXX = g++
CFLAGS = -c -Wall -Wextra -pedantic -std=c++11

GCI: build/GCI.o build/ISOhandler.o build/TextFileParser.o
	$(CXX) build/GCI.o build/ISOhandler.o build/TextFileParser.o -o GCI

build/GCI.o: src/GCI.cpp
	$(CXX) $(CFLAGS) src/GCI.cpp -o build/GCI.o 

build/ISOhandler.o: src/ISOhandler.cpp
	$(CXX) $(CFLAGS) src/ISOhandler.cpp -o build/ISOhandler.o 

build/TextFileParser.o: src/TextFileParser.cpp
	$(CXX) $(CFLAGS) src/TextFileParser.cpp -o build/TextFileParser.o 

clean:
	rm *~ *.o


