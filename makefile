# must be using linux and have openGL, GLUT and Portaudio installed

P = cnoodle

SRCDIR = ./src
TESTDIR = ./src/tests
BUILDDIR = ./build

SOURCES = $(shell ls $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))

LDLIBS =  -L/usr/local/lib -lGL -lGLU -lglut -lportaudio -lasound -lm -lpthread -lglib
CFLAGS = -g -Wall -O3 -pthread -std=gnu11 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
CC = gcc


# Default make
# currently uses test_demo_game.c as default
$(BUILDDIR)/$(P): $(OBJECTS)
	$(CC) $(CFLAGS) -c $(TESTDIR)/test_demo_game.c -o $(BUILDDIR)/%.o
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BUILDDIR)/$(P) $(LDLIBS)


# Make a single .o file without linking
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


# Make tests for a certain component
# (put test c file without directory or .c extension)
test_%: $(OBJECTS)
	$(CC) $(CFLAGS) -c $(TESTDIR)/%.c -o $(BUILDDIR)/%.o
	$(CC) $(CFLAGS) $(BUILDDIR)/%.o -o $(BUILDDIR)/$(P)_test_% $(LDLIBS)

clean:
	rm $(BUILDDIR)/*.o $(BUILDDIR)/$(P)*

