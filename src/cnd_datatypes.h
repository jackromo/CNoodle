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

/*
 * sprite: A series of subimages to be rendered one after another on screen at a position.
 * Has an array of subimages, an x-y position, a number of frames between each subimage, a number of subimages
 * and a current subimage index.
 */
struct sprite {
    int spr_id;
    int x;
    int y;
    int period; // Number of frames between each image, if -1 then static
    int current_img;    // Index of current subimage starting at 0
    int num_imgs;       // Number of subimages.
    int last_subimg_time;   // Number of rames since last subimage.
    GLuint* texture;    // Array of subimage textures
};

/*
 * entity: An atomic element of a room that independently updates.
 * Has a unique ID, an update_self function that is called each update of the game, and a position.
 */
struct entity {
    int id; // Unique identifier for an entity.
    t_update_command_container (*update_self)(t_room*, t_entity*);
    t_sprite current_spr;
    int x;
    int y;
    void *ent_data; // can be used by entity, must be cast to a meaningful struct first
};

/*
 * sound: A single sound to play, eg. a music track or a sound effect.
 * Contains a file path to a sound and a volume from 0 to a positive value.
 */
struct sound {
    int snd_id;
    char* snd_path;
    int volume;
};

// All game data types

struct str_llist_node;
typedef struct str_llist_node llist_node;

typedef enum {
    ENTITY,
    SOUND,
    ROOM,
    SPRITE
} elem_type;

/*
 * llist_node: Node in a linked list.
 */
struct str_llist_node {
    llist_node* next;
    elem_type type;
    void* elem;
};

/*
 * hashtable: Table of linked lists. Indexes elements by ID.
 */
typedef struct {
    int num_elems;  // number of linked lists in hashtable
    llist_node** list;  // all linked lists
    // when multithreading added, put lock for each linked list
} hashtable;

/*
 * game_data: Contains all data about a particular game.
 * Includes all rooms, sprites and sounds, screen size, current room, etc.
 * Is updated each frame by update().
 */
struct game_data {
    /*
     * entities: Array of all entities in game.
     * Only place where entities can be directly referenced.
     * Must remain sorted by ID.
     */
    int num_entities;
    hashtable entities;
    int num_rooms;
    hashtable rooms;
    int num_sprites;
    hashtable sprites;
    int num_sounds;
    hashtable sounds;
    int scr_width;
    int scr_height;
    t_room *current_room;
    int max_id;
};

#endif // CND_DATATYPES_H
