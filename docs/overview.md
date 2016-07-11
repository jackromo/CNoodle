# Internals Overview

This document describes the purpose and internal workings of CNoodle at a high level
of abstraction.

CNoodle is an open source library for 2D game development in C.

## Example CNoodle project

An example project is shown here.

File structure:

/project
    main.c
    /images
        player_idle.png
        background.png
    /sound
        main_theme.png
        player_jump.png

main.c:

 #include "cnoodle.h"

int main() {
    // make central game data struct
    cnd_game_data data = cnd_make_game_data();
    // add entities, sprites and sound here, eg.
    cnd_sprite player_idle_sprite = cnd_make_sprite("./images/player_idle.png");
    // now run the game
    cnd_run(data);
}

The main.c file should be compiled with all of CNoodle's dependencies
and then run in the same directory it was compiled at.

## Game data

A CNoodle game revolves around the central cnd_game_data struct. It
contains all necessary data about the game to run, and is the central
access point for anything about the game.

Everything within the game data is indexed by an ID, which is used to
retrieve said item. If an item with an ID is removed, that ID cannot be
reused; the next item created will have an ID one greater than all other
IDs ever used.

It contains the following main sets of components:

### Entities

An entity is simply an object that performs a user-defined action in the
game, eg. the main player or an enemy. Entities are abstract and
flexible by their nature.

CNoodle entities are unique in that they never actually modify data by
themselves at all; rather, they generate a list of commands that they
return each update, which are collected by the main update loop and
dispatched in a thread-safe manner.

Entities have a set of event handler functions to modify:

* init: Called when the entity has its first update.
* step: Called every update.
* destroy: Called when the entity is deleted.
* draw_before: Called just before the entity is drawn.
* draw_after: Called just after the entity is drawn.

(The draw functions do not return a set of update commands, as they are
run in the render loop instead. They can, however, call draw functions
as all the other functions can, and should be the only functions
generally to do this.)

### Rooms

A room is a container for a set of entities who update together. A room
might be a level of a game or a menu screen.

(NB: The room does not actually contain any entities, only their IDs, so
deleting a room will not delete its contained entities.)

The main game data struct contains the ID of the current room, whose
entities are updated; no entities from other rooms are updated at this
time.

### Sprites

A sprite is a series of images to be drawn at a location on the screen.
A sprite will be reused many times, as each entity remembers its own
sprite so it can be drawn on the screen. For instance, an enemy_walk
sprite may exist to display an enemy's walk cycle, but may be used by
many different entities to display themselves on screen.

Sprites use OpenGL to load and draw themselves. They should be all added
at the start of the game, so that time is not wasted adding them at
runtime.

### Sounds

A sound is a single sample of noise that can be played, paused, resumed
and stopped completely. Sounds use PortAudio to play, and should be
added at the start of the game, so that time is not wasted adding them
at runtime.

## Startup

On startup, CNoodle will take a gamedata struct and start two separate
threads: the update loop and the render loop. The gamedata struct should
contain all necessary data about the game to start.

## Main loops

### Update

In the update loop, CNoodle will first update every entity in the current
room. This will happen in many threads, so entity update code must be
thread safe.

In an update, each entity is checked along certain criteria, to decide
whether to call each event handler function or not on that update. Each
update function will return a series of update commands, which are all
returned together by the entity.

(NB: Each entity is fed a pointer to the main game data struct; this can
be read safely without locks, but the struct must not be written to!)

After collecting all the update commands for the current update, the
loop will then look through each command and feed them into command
dispatcher functions, which will execute each command in a multithreaded
and thread-safe manner.

Certain commands may be optimized out, eg. altering an entity means
nothing if it is being removed in the same update.

### Render

In the render loop, CNoodle will get references to every entity in the
current room, and gather their respective sprites along with info from
the entity pertaining to how to render itself, eg. which frame to show.

After this, the render loop will call the draw_before function of each
entity. The entity will then draw each sprite to the screen in order of
depth, offsetting them by the current camera position, paint the image
to the screen and update the current frame of each entity, acquiring a
lock on each one to do so.

The render loop will end by calling the draw_after function of each
entity that was drawn. After this, the render loop repeats.

## Shutdown

If an entity issues a quit command, the update loop will halt, free all
game data, and finish. The render loop will detect that the game data
pointer is NULL, and also finish.
