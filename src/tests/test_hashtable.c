/*
 * File: test_hashtable.c
 *
 * Testing suite for hashtables.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */


#include "../cnoodle.h"
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    hashtable table;
} hfixture;


void table_setup(hfixture *hf, gconstpointer test_data) {
    hf->table = make_hashtable();
    t_entity *ent1 = make_entity(NULL, 0, 0, NULL);
    ent1->id = 0;
    hashtable_add(*(hf->table), (void *) ent1, ENTITY);
    t_entity *ent2 = make_entity(NULL, 1, 1, (void *) ent1);
    ent2->id = 1;
    hashtable_add(*(hf->table), (void *) ent2, ENTITY);
}

void table_teardown(hfixture *hf, gconstpointer test_data) {
    hashtable_free(hf->table);
}


// TODO: tests here


int main() {
    // TODO: run tests here
}

