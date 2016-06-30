/*
 * File: binarysearch.c
 *
 * Binary search utility function.
 *
 * Author: Jack Romo.
 */

#include <stdio.h>
#include "cnoodle.h"


/*
 * binary_search: Search game_data array for something with a certain ID.
 * Returns item with desired ID if found, null otherwise.
 *
 * elem_array (void**): Array of elements to find element with ID in.
 * id_array (int*): Array of ID's of elements, in same sequence as elements.
 * num_elements (int): Number of elements.
 * desired_id (int): Desired ID of element to be found.
 * get_id (id_func): Function pointer to function that, given an element, gets its ID.
 */
void* binary_search(void** elem_array, int* id_array, int num_elems, int desired_id, id_func get_id) {
    int j = num_elems / 2;
    void* current_elem;
    while (((float) j) / 2f >= 1) {
        int current_id = get_id(elem_array[j]);
        if (current_id == desired_id) {
            return elem_array[j];
        } else if (current_id < desired_id) {
            j += (j / 2 < 1 ? 1 : j / 2);
        } else {
            j -= (j / 2 < 1 ? 1 : j / 2);
        }
    }
    perror("Could not find desired element");
    return -1;
}
