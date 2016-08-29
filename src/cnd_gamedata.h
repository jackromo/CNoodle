/*
 * File: cnd_gamedata.h
 *
 * Header for all game data declarations.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#ifndef CND_GAMEDATA_H
#define CND_GAMEDATA_H

#include "cnd_datatypes.h"
#include "cnd_hashtable.h"

/*
 * game_data: Contains all data about a particular game.
 * Includes all entities rooms, sprites and sounds, screen size, current room, etc.
 * Is updated each frame by update(), and rendered to the screen by render().
 */
struct game_data {
    /*
     * entities: All entities in game.
     * Only place where entities can be directly referenced.
     */
    int num_entities;
    hashtable entities;
    /*
     * rooms: All rooms in game.
     * Only place where rooms can be directly referenced.
     */
    int num_rooms;
    hashtable rooms;
    /*
     * sprites: All sprites in game.
     * Only place where sprites can be directly referenced.
     */
    int num_sprites;
    hashtable sprites;
    /*
     * sounds: All sounds in game.
     * Only place where sounds can be directly referenced.
     */
    int num_sounds;
    hashtable sounds;
    // All metadata about game.
    int scr_width;  // Width and height of screen that user sees, in pixels.
    int scr_height;
    int camera_x;   // X-Y coordinates of top-left corner of camera in room.
    int camera_y;
    int current_room_id;   // ID of current room.
    int max_id;     // Largest ID ever used.
};

// Game data interface commands (see gamedata.c for implementation)

t_game_data make_game_data(char *);
void gamedata_free(t_game_data *);

// Game data element functions (used by dispatchers and entity event handlers)
// (add_ and del_ functions to be used by dispatchers only)

// entity functions
t_entity *get_entity(t_game_data *, int);
void add_entity(t_game_data *, t_entity *);
void del_entity(t_game_data *, int);
int *get_entity_ids(t_game_data *);

// room functions
t_room *get_room(t_game_data *, int);
void add_room(t_game_data *, t_room *);
void del_room(t_game_data *, int);
int *get_room_ids(t_game_data *);

// sprite functions
t_sprite *get_sprite(t_game_data *, int);
void add_sprite(t_game_data *, t_sprite *);
void del_sprite(t_game_data *, int);
int *get_sprite_ids(t_game_data *);

// sound functions
t_sound *get_sound(t_game_data *, int);
void add_sound(t_game_data *, t_sound *);
void del_sound(t_game_data *, int);
int *get_sound_ids(t_game_data *);

int *get_ids(t_game_data *);

int loop_update(t_game_data *);
int loop_render(t_game_data *);

#endif //CND_GAMEDATA_H
