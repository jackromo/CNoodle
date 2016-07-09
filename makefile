# must be using linux and have openGL, GLUT and Portaudio installed

P = cnoodle

SRCDIR = ./src
BUILDDIR = ./build

SOURCES = $(shell ls $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))

LDLIBS = -L/usr/local/lib -lGL -lGLU -lglut -lportaudio -lasound -lm -lpthread
CFLAGS = -g -Wall -O3 -pthread -std=gnu11
CC = gcc

$(BUILDDIR)/$(P): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BUILDDIR)/$(P) $(LDLIBS)

# don't link yet
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(BUILDDIR)/*.o $(BUILDDIR)/$(P)
