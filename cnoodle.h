/*
 * File: cnoodle.h
 *
 * Main header in CNoodle.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#include <GL/gl.h>
#include <portaudio.h>
#include <GL/gl.h>

#ifndef CNOODLE_H
#define CNOODLE_H

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

#include "cnd_gamedata.h"

// All struct definitions

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

// Type of command.
enum command_type {
    ALTER_ENTITY,
    ADD_ENTITY,
    REM_ENTITY,
    ALTER_ROOM,
    NEXT_ROOM,
    PLAY_SND,
    PAUSE_SND,
    END_SND,
    QUIT
};

/*
 * update_command: A request to alter the game's global state.
 * Returned by entities upon running their update_self() functions, and parsed by the global update().
 * Will update variables or flags in game_data.
 * Contains its command_type and a command-specific list of values.
 */

// All data types for details of specific commands.
enum alter_entity_attr {
    CURRENT_SPR, X, Y, UPDATE_SELF, ENT_DATA
};
struct alter_entity_command {
    int target_id;
    t_entity model_ent; // Target entity is modified to be like this one
    enum alter_entity_attr modified_attr;   // Attribute of target entity to modify
};
struct add_entity_command {
    t_entity new_entity;    // Can have any ID, will be set upon creation to be highest existing ID + 1
};
struct rem_entity_command {
    int ent_id;     // ID of entity to remove
};
enum alter_room_attr {
    ENTITIES, WIDTH, HEIGHT
};
struct alter_room_command {
    int target_id;
    t_room model_room;
    enum alter_room_attr modified_attr;
};
struct next_room_command {
    int next_room_id;
};
struct play_sound_command {
    int sound_id;
};
struct pause_sound_command {
    int sound_id;
};
struct end_sound_command {
    int sound_id;
};
enum quit_status {
    NORMAL, ERROR
};
struct quit_command {
    enum quit_status status;
};

struct update_command {
    enum command_type type;
    union {
        struct alter_entity_command alter_ent;
        struct add_entity_command add_ent;
        struct rem_entity_command rem_ent;
        struct alter_room_command alter_room;
        struct next_room_command next_room;
        struct play_sound_command play_snd;
        struct pause_sound_command pause_snd;
        struct end_sound_command end_snd;
        struct quit_command quit;
    } data;
};

/*
 * update_command_container: Contains a series of update commands from a single entity.
 */
struct update_command_container {
    int num_commands;
    t_update_command* commands;
};

// All update command dispatcher functions
void cmd_alter_entity(t_game_data*, struct alter_entity_command);
void cmd_add_entity(t_game_data*, struct add_entity_command);
void cmd_rem_entity(t_game_data*, struct rem_entity_command);
void cmd_alter_room(t_game_data*, struct alter_room_command);
void cmd_next_room(t_game_data*, struct next_room_command);
void cmd_play_sound(t_game_data*, struct play_sound_command);
void cmd_pause_sound(t_game_data*, struct pause_sound_command);
void cmd_end_sound(t_game_data*, struct end_sound_command);
void cmd_quit(t_game_data*, struct quit_command);

#endif //CNOODLE_H
