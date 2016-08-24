prefix=/usr/local
CXX = g++
CFLAGS = -c -Wall -Wextra -pedantic -std=c++11
ARGPFLAGS = -Wno-unused-function -Wno-missing-field-initializers

OBJECTS = build/GCI.o build/ISOhandler.o build/RegionFileParser.o build/CodeAssembler.o build/ObjdumpFileParser.o build/FileOperations.o

AROBJECTS = build/GCI-ar.o build/FileOperations.o build/ObjdumpFileParser.o

all: GCI GCI-ar

GCI: $(OBJECTS)
	$(CXX) $(OBJECTS) -o GCI

GCI-ar: $(AROBJECTS)
	$(CXX) $(AROBJECTS) -o GCI-ar

build/GCI.o: src/GCI.cpp
	$(CXX) $(CFLAGS) $(ARGPFLAGS) src/GCI.cpp -o build/GCI.o 

build/GCI-ar.o: src/GCI-ar.cpp
	$(CXX) $(CFLAGS) src/GCI-ar.cpp -o build/GCI-ar.o 

build/ISOhandler.o: src/ISOhandler.cpp
	$(CXX) $(CFLAGS) src/ISOhandler.cpp -o build/ISOhandler.o 

build/RegionFileParser.o: src/RegionFileParser.cpp
	$(CXX) $(CFLAGS) src/RegionFileParser.cpp -o build/RegionFileParser.o

build/ObjdumpFileParser.o: src/ObjdumpFileParser.cpp
	$(CXX) $(CFLAGS) src/ObjdumpFileParser.cpp -o build/ObjdumpFileParser.o

build/CodeAssembler.o: src/CodeAssembler.cpp
	$(CXX) $(CFLAGS) src/CodeAssembler.cpp -o build/CodeAssembler.o  

build/FileOperations.o: src/FileOperations.cpp
	$(CXX) $(CFLAGS) src/FileOperations.cpp -o build/FileOperations.o  

clean:
	rm -f src/*~ build/*.o GCI GCI-ar

install: GCI GCI-ar
	install GCI-ar $(prefix)/bin
	install GCI $(prefix)/bin
    
.PHONY: install
