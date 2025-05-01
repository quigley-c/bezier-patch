CC=g++
CFLAGS=-c -Wall

LDFLAGS = -lGL -lglut  -lGLU

SOURCES=main.cpp display.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=project

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

main.o: main.cpp display.cpp
	$(CC) $(CFLAGS) $< $(LDFLAGS)

clean:
	rm *.o ${EXECUTABLE}
