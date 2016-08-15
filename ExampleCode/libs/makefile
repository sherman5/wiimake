CC = powerpc-eabi-gcc
AR = powerpc-eabi-ar
CFLAGS = -c
ARFLAGS = -cvq

OBJECTS = ExampleLibrary.o

libexample.a: $(OBJECTS)
	$(AR) $(ARFLAGS) libexample.a $(OBJECTS)

ExampleLibrary.o: ExampleLibrary.c
	$(CC) $(CFLAGS) ExampleLibrary.c -o ExampleLibrary.o

clean:
	rm *~ *.o libexample.a


