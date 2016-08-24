/*
 * File: cnd_hashtable.h
 *
 * Header for hashtables.
 *
 * Author: Jack Rom <sharrackor@gmail.com>
 */

#ifndef CND_HASHTABLE_H
#define CND_HASHTABLE_H

#include "cnd_llist.h"
#include <pthread.h>


/*
 * hashtable: Table of linked lists. Indexes elements by ID.
 */
typedef struct {
    int num_elems;  // number of linked lists in hashtable
    llist_node** list;  // all linked lists
    pthread_mutex_t *mutexes;  // mutexes for each linked list
} hashtable;

// All hashtable functions (see cnd_hashtable.c)

hashtable make_hashtable(int num_elems);
int hash(int id, hashtable table);
void hashtable_add(hashtable table, void* elem, elem_type type);
void *hashtable_get(hashtable table, int id);
void hashtable_del(hashtable table, int id);
bool hashtable_contains(hashtable table, int id);
int *hashtable_get_ids(hashtable table);
int hashtable_get_num_entries(hashtable table);
void hashtable_free(hashtable table);

#endif //CND_HASHTABLE_H
