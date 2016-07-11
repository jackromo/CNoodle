/*
 * File: rooms.c
 *
 * Contains all methods for rooms.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#include "cnoodle.h"
#include <stdlib.h>

t_room make_room(int *entity_ids, int num_entities, int width, int height) {
    t_room room;
    room.room_id = 0;
    room.entity_ids = entity_ids;
    room.num_entities = num_entities;
    room.width = width;
    room.height = height;
    return room;
}

void free_room(t_room *room) {
    // not responsible for deleting entities, also stored in gamedata
    free(room->entity_ids);
    free(room);
}
