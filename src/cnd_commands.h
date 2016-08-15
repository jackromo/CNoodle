/*
 * File: cnd_commands.h
 *
 * All command types and dispatcher declarations.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#ifndef CND_COMMANDS_H
#define CND_COMMANDS_H

#include <glib.h>

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

// All update command dispatcher functions (see dispatchers.c)

void cmd_alter_entity(t_game_data*, struct alter_entity_command);
void cmd_add_entity(t_game_data*, struct add_entity_command);
void cmd_rem_entity(t_game_data*, struct rem_entity_command);
void cmd_alter_room(t_game_data*, struct alter_room_command);
void cmd_next_room(t_game_data*, struct next_room_command);
void cmd_play_sound(t_game_data*, struct play_sound_command);
void cmd_pause_sound(t_game_data*, struct pause_sound_command);
void cmd_end_sound(t_game_data*, struct end_sound_command);
void cmd_quit(t_game_data*, struct quit_command);

/*
 * update_command_container: Contains a series of update commands from a single entity.
 * Stores these as a linked list, which is never traversed beyond the first element; by
 * having the threadpool of command dispatchers only take the first command and move the
 * head pointer one along and having updating entities only append commands to the start,
 * the asymptotic complexity of pushing or popping a command is entirely constant.
 *
 * Update command containers can also be combined by putting the head of one's llist on the
 * tail of the other, which is why each container remembers the address of its last element.
 * This also takes constant time. After all entities have updated, all their containers will
 * be combined in this way and the resulting container is fed to the dispatcher threadpool.
 */
struct update_command_container {
    int num_commands;
    GSList *commands;   // Linked list of update commands (uses standard GLib linked list.)
    GSList *commands_end;   // Last entry in linked list.
};

// All update command container functions (see cmdcontainer.c)

t_update_command_container make_update_command_container();
void push_command(t_update_command_container *);
void pop_command(t_update_command_container *);
void remove_command(t_update_command_container *, t_update_command *);
void append_container(t_update_command_container *, t_update_command_container *);

#endif // CND_COMMANDS_H
