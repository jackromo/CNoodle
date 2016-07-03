/*
 * File: dispatchers.c
 *
 * All update command dispatcher functions.
 * Must support multithreading, ie. must use game data only for reading
 * then acquire a lock on desired struct to write to.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#include "cnoodle.h"


void cmd_alter_entity(t_game_data *data, struct alter_entity_command cmd) {
    t_entity *target_entity = get_entity(data, cmd.target_id);
    // TODO: acquire lock on linked list, modify, then release
    switch(cmd.modified_attr) {
        case CURRENT_SPR:
            target_entity->current_spr = cmd.model_ent.current_spr;
            break;
        case X:
            target_entity->x = cmd.model_ent.x;
            break;
        case Y:
            target_entity->y = cmd.model_ent.y;
            break;
        case UPDATE_SELF:
            target_entity->update_self = cmd.model_ent.update_self;
            break;
        case ENT_DATA:
            target_entity->ent_data = cmd.model_ent.ent_data;
            break;
        default:
            break;
    }
}

void cmd_add_entity(t_game_data *data, struct add_entity_command cmd) {
    //
}

void cmd_rem_entity(t_game_data *data, struct rem_entity_command cmd) {
    //
}

void cmd_alter_room(t_game_data *data, struct alter_room_command cmd) {
    //
}

void cmd_next_room(t_game_data *data, struct next_room_command cmd) {
    //
}

void cmd_play_sound(t_game_data *data, struct play_sound_command cmd) {
    //
}

void cmd_pause_sound(t_game_data *data, struct pause_sound_command cmd) {
    //
}

void cmd_end_sound(t_game_data *data, struct end_sound_command cmd) {
    //
}

void cmd_quit(t_game_data *data, struct quit_command cmd) {
    //
}
