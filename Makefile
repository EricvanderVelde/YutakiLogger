CFLAGS=-O2 -I../influxdb-cpp $(shell pkg-config --cflags libmodbus)
LIBS=$(shell pkg-config --libs libmodbus)

EXEC = YutakiLogger
SOURCES = $(wildcard *.cc)
OBJECTS = $(SOURCES:.cc=.o)
CC=g++

$(EXEC):	$(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) ${LIBS}

%.o: %.cc
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)
