/*
 * File: main.c
 *
 * Currently contains all code in CNoodle.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */


#include <GL/gl.h>
#include <portaudio.h>

struct game_data;
struct entity;
struct room;
struct sprite;
struct sound;
struct update_command;
struct update_command_container;

// TODO: remove t_
typedef struct game_data t_game_data;
typedef struct entity t_entity;
typedef struct room t_room;
typedef struct sprite t_sprite;
typedef struct sound t_sound;
typedef struct update_command t_update_command;
typedef struct update_command_container t_update_command_container;

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
    int num_entities;
    int width;
    int height;
};

/*
 * entity: An atomic element of a room that independently updates.
 * Has a unique ID, an update_self function that is called each update of the game, and a position.
 */
struct entity {
    int id; // Unique identifier for an entity.
    t_update_command_container (*update_self)(t_room*, t_entity*);
    t_sprite current_spr;
    void *ent_data; // can be used by entity, must be cast to a meaningful struct first
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
    ALTER_ROOM,
    NEXT_ROOM,
    PLAY_SND,
    PAUSE_SND,
    END_SND,
    QUIT
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
struct alter_room_command {
    //
};
struct next_room_command {
    //
};
struct play_sound_command {
    //
};
struct pause_sound_command {
    //
};
struct end_sound_command {
    //
};
struct quit_command {
    //
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

/*
 * update: Update the game state by one iteration.
 *
 * Is distinctly separate from rendering, as it merely alters the game's internal data.
 * Works by getting the current room's contained entities, then calling their update_self() functions.
 * Each function returns an update_command_container struct, containing a set of commands to be executed on game_data.
 * These commands are gathered and each executed by command_dispatcher functions, which each take
 * a certain type of update_command and the game_data*, returning nothing and updating the game_data.
 *
 * data (t_game_data *): Pointer to data about game to be updated.
 */
int update(t_game_data* data) {
    t_room current_room = data->current_room;
    t_update_command_container commands[current_room.num_entities];   // FIXME: memory allocation, too slow
    // Get all update commands
    // TODO: multithreading
    for(int i = 0; i < current_room.num_entities; i++) {
        t_entity current_entity = data->entities[i];
        commands[i] = current_entity.update_self(&current_room, &current_entity);
    }
    // Parse all commands
    // (must be done in a separate loop bc. may modify other entities before they update)
    // TODO: multithreading
    for(int i = current_room.num_entities - 1; i > 0; i--) {
        t_update_command_container current_commands = commands[i];
        for(int j = 0; j < current_commands.num_commands; j++) {
            t_update_command command = current_commands.commands[j];
            switch(command.type) {
                // TODO: add command dispatcher function calls
                case ALTER_ENTITY: break;
                case ADD_ENTITY: break;
                case REM_ENTITY: break;
                case ALTER_ROOM: break;
                case NEXT_ROOM: break;
                case PLAY_SND: break;
                case PAUSE_SND: break;
                case END_SND: break;
                case QUIT: break;
                default: break;
            }
        }
    }
    return 0;
}

/*
 * render: Render the game state's next frame.
 *
 * Reads sprites of all entities in current room, updates their subimage if needed, and displays all images.
 *
 * data (t_game_data *): Pointer to data about game to be updated.
 */
void render(t_game_data* data) {
    // TODO
}

int main() {
    return 0;
}
