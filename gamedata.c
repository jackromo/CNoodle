/*
 * File: gamedata.c
 *
 * All utility functions related to the game_data data type.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#include "cnoodle.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/*
 * get_id: Get the ID of a generic entity. Return -1 if not recognized.
 */
int get_id(void* elem, elem_type type) {
    switch(type) {
        case ENTITY:
            return ((t_entity*) elem)->id;
        case SOUND:
            return ((t_sound*) elem)->snd_id;
        case SPRITE:
            return ((t_sprite*) elem)->spr_id;
        case ROOM:
            return ((t_room*) elem)->room_id;
        default:
            return -1;
    }
}

llist_node make_node(void* elem, elem_type type) {
    llist_node node;
    node.elem = elem;
    node.type = type;
    node.next = NULL;
    return node;
}

int get_llist_node_id(llist_node node) {
    return get_id(node.elem, node.type);
}

llist_node get_node(llist_node* start, int id) {
    llist_node* current_node = start;
    while(current_node->next != NULL) {
        if(get_llist_node_id(*current_node) == id) {
            return *current_node;
        } else {
            current_node = current_node->next;
        }
    }
    return *start;
}

void add_node(llist_node** start, llist_node new_node) {
    llist_node *first_node = *start;
    *start = &new_node;
    new_node.next = (first_node != NULL) ? first_node : NULL;
}

void del_node(llist_node** start, int id) {
    llist_node* current_node = (*start)->next;
    llist_node* prev_node = *start;
    if(get_llist_node_id(*prev_node) == id) {
        *start = current_node;
        free(prev_node);
        return;
    }
    while(current_node != NULL) {
        if(get_llist_node_id(*current_node) == id) {
            prev_node->next = current_node->next;
            free(current_node);
            return;
        } else {
            prev_node = current_node;
            current_node = current_node->next;
        }
    }
    perror("Could not find node");
}

bool llist_contains(llist_node* start, int id) {
    llist_node* current_node = start;
    while(current_node->next != NULL) {
        if(get_llist_node_id(*current_node) == id)
            return true;
        current_node = current_node->next;
    }
    return false;
}

int llist_get_length(llist_node* start) {
    llist_node* current_node = start;
    int len = 0;
    while(current_node->next != NULL) {
        len++;
        current_node = current_node->next;
    }
    return len;
}

int *llist_get_all_ids(llist_node* start) {
    llist_node* current_node = start;
    int *ids = malloc(sizeof(int)*llist_get_length(start));
    if(ids == NULL) exit(EXIT_FAILURE);
    for(int i=0; current_node->next != NULL; i++) {
        ids[i] = get_id(current_node->elem, current_node->type);
        current_node = current_node->next;
    }
    return ids;
}

hashtable make_hashtable(int num_elems) {
    hashtable table;
    llist_node** list = (llist_node**) malloc(sizeof(llist_node*)*num_elems);
    if(list == NULL) {
        perror("Could not allocate list for hashtable.");
        exit(EXIT_FAILURE);
    }
    table.list = list;
    table.num_elems = num_elems;
    return table;
}

int hash(int id, hashtable table) {
    return id % (table.num_elems);
}

void hashtable_add(hashtable table, void* elem, elem_type type) {
    llist_node elem_node = make_node(elem, type);
    int entry_pos = hash(get_id(elem, type), table);
    add_node(&table.list[entry_pos], elem_node);
}

void *hashtable_get(hashtable table, int id) {
    int entry_pos = hash(id, table);
    return get_node(table.list[entry_pos], id).elem;
}

void hashtable_del(hashtable table, int id) {
    int entry_pos = hash(id, table);
    del_node(&table.list[entry_pos], id);
}

bool hashtable_contains(hashtable table, int id) {
    for(int i = 0; i < table.num_elems; i++) {
        if(llist_contains(table.list[i], id))
            return true;
    }
    return false;
}

int *hashtable_get_ids(hashtable table) {
    int **id_arrays = (int **) malloc(sizeof(int*)*table.num_elems);
    if(id_arrays == NULL) exit(EXIT_FAILURE);
    int lengths[table.num_elems];
    int sum_lengths = 0;
    for(int i = 0; i < table.num_elems; i++) {
        lengths[i] = llist_get_length(table.list[i]);
        sum_lengths += lengths[i];
        id_arrays[i] = (int *) malloc(sizeof(int)*lengths[i]);
        if(id_arrays[i] == NULL) exit(EXIT_FAILURE);
        id_arrays[i] = llist_get_all_ids(table.list[i]);
    }
    int *ids = (int *) malloc(sizeof(int)*sum_lengths);
    if(ids == NULL) exit(EXIT_FAILURE);
    int index = 0;
    for(int i = 0; i < table.num_elems; i++) {
        for(int j = 0; j < lengths[i]; j++) {
            ids[index] = id_arrays[i][j];
            index++;
        }
    }
    return ids;
}

int hashtable_get_num_entries(hashtable table) {
    int length = 0;
    for(int i = 0; i < table.num_elems; i++) {
        length += llist_get_length(table.list[i]);
    }
    return length;
}

// game data

t_game_data make_game_data(char* fname) {
    // TODO: parse fname for game data, game data currently starts out empty
    int num_hashtable_entries = 64;     // temp value, should be read from fname
    t_game_data data;
    data.num_entities = 0;
    data.entities = make_hashtable(num_hashtable_entries);
    data.num_rooms = 0;
    data.rooms = make_hashtable(num_hashtable_entries);
    data.num_sounds = 0;
    data.sounds = make_hashtable(num_hashtable_entries);
    data.num_sprites = 0;
    data.sprites = make_hashtable(num_hashtable_entries);
    data.scr_width = data.scr_height = 400;     // temp value
    return data;
}

t_entity *get_entity(t_game_data *data, int id) {
    return (t_entity *) hashtable_get(data->entities, id);
}

void add_entity(t_game_data *data, t_entity entity) {
    data->max_id = entity.id = data->max_id + 1;
    data->num_entities++;
    hashtable_add(data->entities, (void*) &entity, ENTITY);
}

void del_entity(t_game_data *data, int id) {
    hashtable_del(data->entities, id);
    data->num_entities--;
}

int *get_entity_ids(t_game_data *data) {
    return hashtable_get_ids(data->entities);
}

t_room *get_room(t_game_data *data, int id) {
    return (t_room *) hashtable_get(data->rooms, id);
}

void add_room(t_game_data *data, t_room room) {
    data->max_id = room.room_id = data->max_id + 1;
    data->num_rooms++;
    hashtable_add(data->rooms, (void*) &room, ROOM);
}

void del_room(t_game_data *data, int id) {
    hashtable_del(data->rooms, id);
    data->num_rooms--;
}

int *get_room_ids(t_game_data *data) {
    return hashtable_get_ids(data->rooms);
}

t_sprite *get_sprite(t_game_data *data, int id) {
    return (t_sprite *) hashtable_get(data->sprites, id);
}

void add_sprite(t_game_data *data, t_sprite sprite) {
    data->max_id = sprite.spr_id = data->max_id + 1;
    data->num_sprites++;
    hashtable_add(data->sprites, (void*) &sprite, SPRITE);
}

void del_sprite(t_game_data *data, int id) {
    hashtable_del(data->sprites, id);
    data->num_sprites--;
}

int *get_sprite_ids(t_game_data *data) {
    return hashtable_get_ids(data->sprites);
}

t_sound *get_sound(t_game_data *data, int id) {
    return (t_sound *) hashtable_get(data->sounds, id);
}

void add_sound(t_game_data *data, t_sound sound) {
    data->max_id = sound.snd_id = data->max_id + 1;
    data->num_sounds++;
    hashtable_add(data->sounds, (void*) &sound, SOUND);
}

void del_sound(t_game_data *data, int id) {
    hashtable_del(data->sounds, id);
    data->num_sounds--;
}

int *get_sound_ids(t_game_data *data) {
    return hashtable_get_ids(data->sounds);
}

int get_num_ids(t_game_data *data) {
    return data->num_entities + data->num_rooms + data->num_sprites + data->num_sounds;
}

int *get_ids(t_game_data *data) {
    int *id_arrays[4] = {
            get_entity_ids(data), get_room_ids(data),
            get_sound_ids(data), get_sprite_ids(data)
    };
    int lengths[4] = {
            data->num_entities, data->num_rooms,
            data->num_sounds, data->num_sprites
    };
    int *ids = malloc(sizeof(int)*get_num_ids(data));
    int index = 0;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < lengths[i]; j++)
            ids[index] = id_arrays[i][j];
    }
    return ids;
}

/*
 * update_loop: Repeatedly update the game state by one iteration.
 *
 * Is distinctly separate from rendering, as it merely alters the game's internal data.
 * Works by getting the current room's contained entities, then calling their update_self() functions.
 * Each function returns an update_command_container struct, containing a set of commands to be executed on game_data.
 * These commands are gathered and each executed by command_dispatcher functions, which each take
 * a certain type of update_command and the game_data*, returning nothing and updating the game_data.
 *
 * data (t_game_data *): Pointer to data about game to be updated.
 */
int loop_update(t_game_data* data) {
    bool has_game_ended = false;
    while(!has_game_ended) {
        t_room current_room = data->current_room;
        t_update_command_container commands[current_room.num_entities];   // FIXME: memory allocation, too slow
        // Get all update commands
        // TODO: thread pool, currently takes O(n*log(num_entities_overall) + update_self_time) time
        for (int i = 0; i < current_room.num_entities; i++) {
            int current_entity_id = current_room.entity_ids[i];
            // TODO: cache all entities in current room
            t_entity *current_entity = get_entity(data, current_entity_id);
            if (hashtable_contains(data->entities, current_entity_id)) {
                commands[i] = (*current_entity).update_self(&current_room, current_entity);
            }
        }
        // Parse all commands
        // (must be done in a separate loop bc. may modify other entities before they update)
        // TODO: multithreading with thread pool
        for (int i = current_room.num_entities - 1; i > 0; i--) {
            t_update_command_container current_commands = commands[i];
            for (int j = 0; j < current_commands.num_commands; j++) {
                t_update_command command = current_commands.commands[j];
                switch (command.type) {
                    case ALTER_ENTITY:
                        cmd_alter_entity(data, command.data.alter_ent);
                        break;
                    case ADD_ENTITY:
                        cmd_add_entity(data, command.data.add_ent);
                        break;
                    case REM_ENTITY:
                        cmd_rem_entity(data, command.data.rem_ent);
                        break;
                    case ALTER_ROOM:
                        cmd_alter_room(data, command.data.alter_room);
                        break;
                    case NEXT_ROOM:
                        cmd_next_room(data, command.data.next_room);
                        break;
                    case PLAY_SND:
                        cmd_play_sound(data, command.data.play_snd);
                        break;
                    case PAUSE_SND:
                        cmd_pause_sound(data, command.data.pause_snd);
                        break;
                    case END_SND:
                        cmd_end_sound(data, command.data.end_snd);
                        break;
                    case QUIT:
                        cmd_quit(data, command.data.quit);
                        has_game_ended = true;
                        break;
                    default:
                        break;
                }
            }
        }
        // TODO: slow loop if updating too fast
    }
    return 0;
}

/*
 * loop_render: Repeatedly render the game state and display it on screen.
 *
 * Reads sprites of all entities in current room, updates their subimage if needed, and displays all images.
 *
 * data (t_game_data *): Pointer to data about game to be rendered.
 */
int loop_render(t_game_data* data) {
    for(;;) {
        // TODO
    }
    return 0;
}
