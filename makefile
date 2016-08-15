# must be using linux and have openGL, GLUT and Portaudio installed

P = cnoodle

SRCDIR = ./src
BUILDDIR = ./build

SOURCES = $(shell ls $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))

LDLIBS =  -L/usr/local/lib -lGL -lGLU -lglut -lportaudio -lasound -lm -lpthread -lglib
CFLAGS = -g -Wall -O3 -pthread -std=gnu11 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
CC = gcc

$(BUILDDIR)/$(P): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BUILDDIR)/$(P) $(LDLIBS)

# don't link yet
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(BUILDDIR)/*.o $(BUILDDIR)/$(P)
