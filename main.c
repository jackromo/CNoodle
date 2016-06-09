/*
 * File: main.c
 *
 * Currently contains all code in CNoodle.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */


#include <stdio.h>
#include <GL/gl.h>
#include <portaudio.h>

struct game_data;
struct entity;
struct room;
struct sprite;
struct sound;
struct hitbox;
struct update_command;

// TODO: remove t_
typedef struct game_data t_game_data;
typedef struct entity t_entity;
typedef struct room t_room;
typedef struct sprite t_sprite;
typedef struct sound t_sound;
typedef struct hitbox t_hitbox;
typedef struct update_command t_update_command;

/*
 * game_data: Contains all data about a particular game.
 * Includes all rooms, sprites and sounds, screen size, current room, etc.
 * Is updated each frame by update().
 */
 struct game_data {
    t_entity* entities;
    t_room* rooms;
    t_sprite* sprites;
    t_sound* sounds;
    int scr_width;
    int scr_height;
    t_room current_room;
};

/*
 * room: Contains a screen's worth of entities, eg. a pause menu or a level.
 * Only one room is focused on at a time, and its entities are all updated each frame.
 * All of a room's entities are also rendered in the render loop.
 * Coordinates are relative to the top-left corner of the room; entities can leave this area.
 */
struct room {
    t_entity* entities; // All entities inside of room
    int width;
    int height;
};

/*
 * entity: An atomic element of a room that independently updates.
 * Has a unique ID, an update_self function that is called each update of the game, and a position.
 */
struct entity {
    int id; // Unique identifier for an entity.
    t_entity (*update_self)(t_room*, t_entity*);
    t_sprite spr;
    t_sound* sounds;
};

/*
 * sprite: A series of subimages to be rendered one after another on screen at a position.
 * Has an array of subimages, an x-y position, a number of frames between each subimage, a number of subimages
 * and a current subimage index.
 */
struct sprite {
    int x;
    int y;
    int period; // Number of frames between each image, if -1 then static
    int current_img;    // Index of current subimage starting at 0
    int num_imgs;       // Number of subimages.
    int last_subimg_time;   // Number of rames since last subimage.
    GLuint* texture;    // Array of subimage textures
};

/*
 * sound: A single sound to play, eg. a music track or a sound effect.
 * Contains a file path to a sound and a volume from 0 to a positive value.
 */
struct sound {
    char* snd_path;
    int volume;
};

/*
 * hitbox: A rectangular object that can collide with other hitboxes.
 * If a hitbox collides with another hitbox, the other hitbox is added to said hitbox's 'collided' array.
 * Contains its position, dimensions, and an array of hitboxes it is currently in contact with.
 */
struct hitbox {
    int x;
    int y;
    int width;
    int height;
    t_hitbox* collided;
};

/*
 * update_command: A request to alter the game's global state.
 * Returned by entities upon running their update_self() functions, and parsed by the global update().
 * Will update variables or flags in game_data.
 * Contains its command_type and a command-specific list of values.
 */

// Type of command.
enum command_type {
    ALTER_ENTITY,
    ADD_ENTITY,
    REM_ENTITY,
    ALTER_LEVEL,
    PLAY_SND,
    PAUSE_SND,
    END_SND
};

// All data types for details of specific commands.
struct alter_entity_command {
    //
};
struct add_entity_command {
    //
};
struct rem_entity_command {
    //
};
struct alter_level_command{
    //
};
struct play_sound_command{
    //
};
struct pause_sound_command{
    //
};
struct end_sound_command{
    //
};

struct update_command {
    enum command_type type;
    union {
        struct alter_entity_command alter_ent;
        struct add_entity_command add_ent;
        struct rem_entity_command rem_ent;
        struct alter_level_command alter_lev;
        struct play_sound_command play_snd;
        struct pause_sound_command pause_snd;
        struct end_sound_command end_snd;
    } data;
};

/*
 * update: Update the game state by one iteration.
 *
 * Is distinctly separate from rendering, as it merely alters the game's internal data.
 * Works by getting the current room's contained entities, then calling their update_self() functions.
 * Each function returns an update_command struct, containing a command to be executed on game_data.
 * These commands are gathered and each executed by command_dispatcher functions, which each take
 * a certain type of update_command and the game_data*, returning nothing and updating the game_data.
 *
 * data (t_game_data *): Pointer to data about game to be updated.
 */
void update(t_game_data* data) {
    // TODO
}

/*
 * render: Render the game state's next frame.
 *
 * Reads sprites of all entities in current level, updates their subimage if needed, and displays all images.
 *
 * data (t_game_data *): Pointer to data about game to be updated.
 */
void render(t_game_data* data) {
    // TODO
}

int main() {
    return 0;
}
