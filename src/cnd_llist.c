/*
 * File: cnd_llist.c
 *
 * Contains all source code for linked lists.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#include "cnoodle.h"
#include "cnd_llist.h"
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
            // FIXME: some nodes contain elems with pointers, need more refined delete
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

void llist_free(llist_node *start) {
    while(start != NULL) {
        del_node(&start, get_llist_node_id(*start));
    }
}
