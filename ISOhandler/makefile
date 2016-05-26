CXX = g++
CFLAGS = -c -Wall -Wextra -pedantic -std=c++11

ISObuilder: ISObuilder.o ISOhandler.o
	$(CXX) ISObuilder.o ISOhandler.o -o ISObuilder

OriginalCodeGenerator: OriginalCodeGenerator.o ISOhandler.o
	$(CXX) OriginalCodeGenerator.o ISOhandler.o -o OriginalCodeGenerator

OriginalCodeGenerator.o: OriginalCodeGenerator.cpp
	$(CXX) $(CFLAGS) OriginalCodeGenerator.cpp -o OriginalCodeGenerator.o 

ISObuilder.o: ISObuilder.cpp
	$(CXX) $(CFLAGS) ISObuilder.cpp -o ISObuilder.o 

ISOhandler.o: ISOhandler.cpp
	$(CXX) $(CFLAGS) ISOhandler.cpp -o ISOhandler.o 

clean:
	rm *~ *.o


