/*
 * File: entities.c
 *
 * Contains all methods for entities.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#include "cnoodle.h"
#include <stdlib.h>

t_entity make_entity(int current_spr_id, int x, int y, void *ent_data) {
    t_entity entity;
    entity.id = 0;
    ent_func_vtable event_handlers;     // All event handlers begin uninitialized
    entity.event_handlers = event_handlers;
    entity.current_spr_id = current_spr_id;
    entity.spr_period = -1;
    entity.spr_current_img = 0;
    entity.spr_last_subimg_time = 0;
    entity.x = x;
    entity.y = y;
    entity.ent_data = ent_data;
    return entity;
}

void free_entity(t_entity *entity) {
    free(entity->ent_data);
    free(entity);
}

t_update_command_container update_entity(t_game_data *data, t_entity *entity) {
    // TODO, dummy code for now
    t_update_command_container container;
    return container;
}
