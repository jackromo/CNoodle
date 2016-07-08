# must be using linux and have openGL, GLUT and Portaudio installed

P = cnoodle
SOURCES = main.c gamedata.c dispatchers.c
OBJECTS = $(SOURCES:.c=.o)
LDLIBS = -L/usr/local/lib -lGL -lGLU -lglut -lportaudio -lasound -lm -lpthread
CFLAGS = -g -Wall -O3 -pthread -std=gnu11
CC = gcc

# use default rule for each object
$(P): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(P) $(LDLIBS)

clean:
	rm *o $(P)
