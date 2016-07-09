/*
 * File: cnd_commands.h
 *
 * All command types and dispatcher declarations.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
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
