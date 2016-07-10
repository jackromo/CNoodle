/*
 * File: cnd_hashtable.c
 *
 * Contains all source code for hashtables.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#include "cnd_hashtable.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

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

void hashtable_free(hashtable table) {
    for(int i = 0; i < table.num_elems; i++) {
        llist_free(table.list[i]);
    }
}
