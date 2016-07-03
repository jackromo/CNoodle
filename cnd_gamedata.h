//
// Created by sharr_000 on 7/3/2016.
//

#ifndef CNOODLE_CND_GAMEDATA_H
#define CNOODLE_CND_GAMEDATA_H

struct str_llist_node;
typedef struct str_llist_node llist_node;

typedef enum {
    ENTITY,
    SOUND,
    ROOM,
    SPRITE
} elem_type;

/*
 * llist_node: Node in a linked list.
 */
struct str_llist_node {
    llist_node* next;
    elem_type type;
    void* elem;
};

/*
 * hashtable: Table of linked lists. Indexes elements by ID.
 */
typedef struct {
    int num_elems;  // number of linked lists in hashtable
    llist_node** list;  // all linked lists
    // when multithreading added, put lock for each linked list
} hashtable;

t_game_data make_game_data(char *);

// entity functions
t_entity *get_entity(t_game_data *, int);
void add_entity(t_game_data *, t_entity);
void del_entity(t_game_data *, int);
int *get_entity_ids(t_game_data *);

// room functions
t_room *get_room(t_game_data *, int);
void add_room(t_game_data *, t_room);
void del_room(t_game_data *, int);
int *get_room_ids(t_game_data *);

// sprite functions
t_sprite *get_sprite(t_game_data *, int);
void add_sprite(t_game_data *, t_sprite);
void del_sprite(t_game_data *, int);
int *get_sprite_ids(t_game_data *);

// sound functions
t_sound *get_sound(t_game_data *, int);
void add_sound(t_game_data *, t_sound);
void del_sound(t_game_data *, int);
int *get_sound_ids(t_game_data *);

int *get_ids(t_game_data *);

int loop_update(t_game_data *);
int loop_render(t_game_data *);

#endif //CNOODLE_CND_GAMEDATA_H
