/*
 * File: cnd_llist.h
 *
 * Header for linked lists.
 *
 * Author: Jack Rom <sharrackor@gmail.com>
 */

#ifndef CND_LLIST_H
#define CND_LLIST_H

#include <stdbool.h>

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


int get_id(void* elem, elem_type type);

llist_node make_node(void* elem, elem_type type);

int get_llist_node_id(llist_node node);

llist_node get_node(llist_node* start, int id);

void add_node(llist_node** start, llist_node new_node);

void del_node(llist_node** start, int id);

bool llist_contains(llist_node* start, int id);

int llist_get_length(llist_node* start);

int *llist_get_all_ids(llist_node* start);

void llist_free(llist_node *start);

#endif //CND_LLIST_H
