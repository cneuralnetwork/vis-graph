CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LIBS = -lglut -lGL -lGLU -lm

TARGET = graph_viewer
SOURCES = main.c graph_ops.c layout.c render.c input.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)

%.o: %.c graph.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

install:
	cp $(TARGET) /usr/local/bin/

.PHONY: all clean install 