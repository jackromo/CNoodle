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

// all type declarations
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
typedef struct update_command t_update_command;
typedef struct update_command_container t_update_command_container;

/*
 * room: Contains a screen's worth of entities, eg. a pause menu or a level.
 * Only one room is focused on at a time, and its entities are all updated each frame.
 * All of a room's entities are also rendered in the render loop.
 * Coordinates are relative to the top-left corner of the room; entities can leave this area.
 */
struct room {
    int room_id;
    int* entity_ids; // IDs of all entities inside of room
    int num_entities;
    int width;
    int height;
};

t_room make_room(int*, int, int, int);
void free_room(t_room *);

/*
 * sprite: A series of subimages to be rendered one after another on screen at a position.
 * Has an array of subimages, an x-y position, a number of frames between each subimage, a number of subimages
 * and a current subimage index.
 */
struct sprite {
    int spr_id;
    int period; // Number of frames between each image, if -1 then static
    int num_imgs;       // Number of subimages.
    GLuint* texture;    // Array of subimage textures
};

t_sprite make_sprite(int, int, GLuint*);
void free_sprite(t_sprite *);
void draw_sprite(t_sprite * /* add args needed when rendering finished */);

/*
 * entity: An atomic element of a room that independently updates.
 * Has a unique ID, an update_self function that is called each update of the game, and a position.
 */
struct entity {
    int id; // Unique identifier for an entity.
    t_update_command_container (*update_self)(t_room*, t_entity*);
    int current_spr_id;
    int current_img;    // Index of current sprite subimage starting at 0
    int last_subimg_time;   // Number of frames since last sprite subimage.
    int x;
    int y;
    void *ent_data; // can be used by entity, must be cast to a meaningful struct first
};

t_entity make_entity(/* TODO */);
void free_entity(t_entity *);
t_update_command_container update_entity(t_entity *); // TODO: deprecate update_self in favor of vtable of callbacks

/*
 * sound: A single sound to play, eg. a music track or a sound effect.
 * Contains a file path to a sound and a volume from 0 to a positive value.
 */
struct sound {
    int snd_id;
    char* snd_path;
    int volume;
};

t_sound make_sound(int, char*, int);
void free_sound(t_sound *);
void play_sound(t_sound *);
void pause_sound(t_sound *);
void stop_sound(t_sound *);

#endif // CND_DATATYPES_H
