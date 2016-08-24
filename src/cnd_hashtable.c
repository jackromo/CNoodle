/*
 * File: cnd_hashtable.c
 *
 * Contains all source code for hashtables.
 * Hashtables are used by t_game_data to store entities, rooms, etc.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#include "cnd_hashtable.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/*
 * make_hashtable: Create a hashtable.
 *
 * num_elems (int): Number of linked lists in hashtable.
 *
 * Returns (hashtable): Empty hashtable.
 */
hashtable make_hashtable(int num_elems) {
    hashtable table;
    llist_node** list = malloc(sizeof(llist_node*) * num_elems);
    if(list == NULL) {
        perror("Could not allocate list for hashtable.");
        exit(EXIT_FAILURE);
    }
    table.list = list;
    table.num_elems = num_elems;
    table.mutexes = malloc(sizeof(pthread_mutex_t) * num_elems);
    if(table.mutexes == NULL) {
        perror("Could not allocate mutexes for hashtable.");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < table.num_elems; i++) {
        pthread_mutex_init(&table.mutexes[i], NULL);
    }
    return table;
}

/*
 * hash: Private method for hashing an ID.
 */
int hash(int id, hashtable table) {
    return id % (table.num_elems);
}

/*
 * hashtable_trylock_id: Acquire lock on llist in hashtable responsible for 'id'.
 * Stall until this is accomplished.
 */
void hashtable_lock_id(hashtable table, int id) {
    pthread_mutex_lock(&table.mutexes[hash(id, table)]);
}

/*
 * hashtable_trylock_id: Attempt to acquire lock on llist in hashtable responsible for 'id'.
 *
 * Will try once; if fails, will return nonzero value.
 */
int hashtable_trylock_id(hashtable table, int id) {
    return pthread_mutex_trylock(&table.mutexes[hash(id, table)]);
}

/*
 * hashtable_unlock_id: Release lock on an ID.
 */
void hashtable_unlock_id(hashtable table, int id) {
    pthread_mutex_unlock(&table.mutexes[hash(id, table)]);
}

/*
 * hashtable_add: Add an element to a hashtable of any type.
 */
void hashtable_add(hashtable table, void* elem, elem_type type) {
    llist_node elem_node = make_node(elem, type);
    int entry_pos = hash(get_id(elem, type), table);
    add_node(&table.list[entry_pos], elem_node);
}

/*
 * hashtable_get: Get a void* to an element with an ID in a hashtable.
 */
void *hashtable_get(hashtable table, int id) {
    int entry_pos = hash(id, table);
    return get_node(table.list[entry_pos], id).elem;
}

/*
 * hashtable_del: Delete an element with an ID in a hashtable.
 */
void hashtable_del(hashtable table, int id) {
    int entry_pos = hash(id, table);
    del_node(&table.list[entry_pos], id);
}

/*
 * hashtable_contains: Return true if contains an ID, false otherwise.
 */
bool hashtable_contains(hashtable table, int id) {
    for(int i = 0; i < table.num_elems; i++) {
        if(llist_contains(table.list[i], id))
            return true;
    }
    return false;
}

/*
 * hashtable_get_ids: Get array of all IDs in hashtable.
 */
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

/*
 * hashtable_get_num_entries: Get number of entries in hashtable.
 */
int hashtable_get_num_entries(hashtable table) {
    int length = 0;
    for(int i = 0; i < table.num_elems; i++) {
        length += llist_get_length(table.list[i]);
    }
    return length;
}

/*
 * hashtable_free: Free all memory in hashtable.
 */
void hashtable_free(hashtable table) {
    for(int i = 0; i < table.num_elems; i++) {
        llist_free(table.list[i]);
    }
    free(table.mutexes);
}
