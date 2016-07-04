/*
 * File: dispatchers.c
 *
 * All update command dispatcher functions.
 * Must support multithreading, ie. must use game data only for reading
 * then acquire a lock on desired struct to write to.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#include <stdlib.h>
#include <stdbool.h>
#include "cnoodle.h"

// TODO: acquire lock on needed data, modify, then release
// only hold one lock at a time

void cmd_alter_entity(t_game_data *data, struct alter_entity_command cmd) {
    t_entity *target_entity = get_entity(data, cmd.target_id);
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
    add_entity(data, cmd.new_entity);
}

void cmd_rem_entity(t_game_data *data, struct rem_entity_command cmd) {
    del_entity(data, cmd.ent_id);
    // remove entity id from rooms
    int *room_ids = get_room_ids(data);
    for(int i = 0; i < data->num_rooms; i++) {
        t_room *room = get_room(data, room_ids[i]);
        bool shifting_ids = false;
        for(int j = 0; j < room->num_entities; j++) {
            if(room->entity_ids[j] == cmd.ent_id) {
                free(room->entity_ids + j);
                shifting_ids = true;
            } else if(shifting_ids && j < room->num_entities-1) {
                room->entity_ids[j] = room->entity_ids[j+1];
            }
        }
        room->num_entities--;
        free(room->entity_ids + room->num_entities - 1);    // last element is empty bc. length decremented
    }
}

void cmd_alter_room(t_game_data *data, struct alter_room_command cmd) {
    t_room *room = get_room(data, cmd.target_id);
    int *model_ent_ids = cmd.model_room.entity_ids;
    int model_num_ents = cmd.model_room.num_entities;
    switch(cmd.modified_attr) {
        case ENTITIES:
            // free old entity ids, make sure not freeing ids intersecting with new entity ids
            for(int *i = room->entity_ids;
                    i < model_ent_ids && i < (room->entity_ids + room->num_entities);
                    i++)
                free(i);    // free everything in room up to model_ent_ids
            for(int *i = model_ent_ids + model_num_ents + 1;
                    i < (room->entity_ids + room->num_entities);
                    i++)
                free(i);    // free everything in room after all model_ent_ids, if any
            room->entity_ids = cmd.model_room.entity_ids;
            break;
        case WIDTH:
            room->width = cmd.model_room.width;
            break;
        case HEIGHT:
            room->height = cmd.model_room.height;
            break;
        default:
            break;
    }
}

void cmd_next_room(t_game_data *data, struct next_room_command cmd) {
    data->current_room = get_room(data, cmd.next_room_id);
}

// cannot do sound-based dispatchers until audio finished

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
    // cleanup code, delete everything
    int *room_ids = get_room_ids(data);
    for(int i = 0; i < data->num_rooms; i++)
        free(get_room(data, room_ids[i]));

    int *entity_ids = get_entity_ids(data);
    for(int i = 0; i < data->num_entities; i++)
        free(get_entity(data, entity_ids[i]));

    int *sprite_ids = get_sprite_ids(data);
    for(int i = 0; i < data->num_sprites; i++)
        free(get_sprite(data, sprite_ids[i]));

    int *sound_ids = get_sound_ids(data);
    for(int i = 0; i < data->num_sounds; i++)
        free(get_sound(data, sound_ids[i]));
}
