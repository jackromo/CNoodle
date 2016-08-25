/*
 * File: cnd_datatypes.h
 *
 * Contains all main data types, eg. room, entity, etc.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#include <portaudio.h>
#include <GL/gl.h>

#ifndef CND_DATATYPES_H
#define CND_DATATYPES_H

// All type declarations

struct game_data;
struct entity;
struct room;
struct sprite;
struct sound;
struct update_command;
struct update_command_container;

typedef struct game_data t_game_data;
typedef struct entity t_entity;
typedef struct room t_room;
typedef struct sprite t_sprite;
typedef struct sound t_sound;
// Declared here for entity update, defined in cnd_commands.h
typedef struct update_command t_update_command;
typedef struct update_command_container t_update_command_container;

/*
 * room: Contains a screen's worth of entities, eg. a pause menu or a level.
 * Only one room is focused on at a time, and its entities are all updated each frame.
 * All of a room's entities are also rendered in the render loop.
 * Coordinates are relative to the top-left corner of the room; entities can leave this area.
 */
struct room {
    int room_id;    // ID of the room.
    int* entity_ids; // IDs of all entities inside of room
    int num_entities;   // Number of entities in room.
    int width;      // Width of room in pixels.
    int height;     // Height of room in pixels.
};

// Room functions (see rooms.c)

t_room make_room(int*, int, int, int);
void free_room(t_room *);

/*
 * sprite: A series of subimages to be rendered one after another on screen at a position.
 * Has an array of subimages and an ID.
 * A sprite will be used by an entity to represent itself on screen, who is responsible for
 * deciding which frame to use and when to show it.
 */
struct sprite {
    int spr_id;         // ID of sprite.
    int num_imgs;       // Number of subimages.
    GLuint* texture;    // Array of subimage textures
};

// Sprite functions (see sprites.c)

t_sprite make_sprite(int, GLuint*);
void free_sprite(t_sprite *);
void draw_sprite(t_sprite * /* add args needed when rendering finished */);

/*
 * ent_func_vtable: A vtable of every function that an entity needs.
 * All functions are used as event handlers, eg. init() is called on an entity's first update.
 */
typedef struct {
    // init: Called on the first update of the game.
    t_update_command_container (*init)(t_game_data const*, t_entity const*);
    // step: Called every update of the game.
    t_update_command_container (*step)(t_game_data const*, t_entity const*);
    // destroy: Called on the update just before the entity is removed.
    t_update_command_container (*destroy)(t_game_data const*, t_entity const*);
    // collide: Called if the entity's sprite intersects with another entity's sprite.
    t_update_command_container (*collide)(t_game_data const*, t_entity const*, int);
    // key_pressed: Called if a key is being held down during the current update.
    t_update_command_container (*key_pressed)(t_game_data const*, t_entity const* /* TODO */);
    // draw_begin: Called in the render loop, just before the entity's sprite is retrieved.
    t_update_command_container (*draw_begin)(t_game_data const*, t_entity const*);
    // draw_end: Called just after all sprites have been draw to the screen, but before the screen is displayed.
    t_update_command_container (*draw_end)(t_game_data const*, t_entity const*);
} ent_func_vtable;

/*
 * entity: An atomic element of a room that independently updates.
 * Has a unique ID, a vtable of event handler functions called conditionally
 * each update, data about its current sprite, a position, and a void* to use
 * as scratchpad space, eg. a player's health, speed, inventory, etc.
 */
struct entity {
    int id; // Unique identifier for an entity.
    ent_func_vtable event_handlers;     // Vtable of event handler functions.
    int current_spr_id;     // ID of the entity's current sprite.
    int spr_period;     // Number of frames between each sprite subimage, if -1 then static
    int spr_current_img;    // Index of current sprite subimage starting at 0
    int spr_last_subimg_time;   // Number of frames since last sprite subimage.
    int x;  // X-Y coordinates of the entity in the room. (Y = down, X = right)
    int y;
    void *ent_data; // can be used by entity, must be cast to a meaningful struct first
};

// Entity functions (see entities.c)

t_entity make_entity(int, int, int, void *);
void free_entity(t_entity *);
t_update_command_container update_entity(t_game_data*, t_entity *); // TODO: deprecate update_self in favor of vtable of callbacks
void draw_entity(t_entity * /* TODO */);

/*
 * sound: A single sound to play, eg. a music track or a sound effect.
 * Contains a file path to a sound and a volume from 0 to a positive value.
 */
struct sound {
    int snd_id;     // ID of the sound.
    char* snd_path;     // Path to the sound source file, relative to the main executable.
    int volume;     // Volume of the sound in decibels.
};

// Sound functions (see sounds.c)

t_sound make_sound(char*, int);
void free_sound(t_sound *);
void play_sound(t_sound *);
void pause_sound(t_sound *);
void stop_sound(t_sound *);

#endif // CND_DATATYPES_H
