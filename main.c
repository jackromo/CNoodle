/*
 * File: main.c
 *
 * Definitions for update and render functions.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#include <GL/gl.h>
#include <portaudio.h>
#include <stdbool.h>
#include "cnoodle.h"

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
            // Binary search for entity with ID
            // TODO: cache all entities in current room
            int j = data->num_entities / 2;
            t_entity current_entity;
            while (((float) j) / 2f >= 1) {
                if (data->entities[j].id == current_entity_id) {
                    current_entity = data->entities[i];
                    break;
                } else if (data->entities[j].id < current_entity_id) {
                    j += (j / 2 < 1 ? 1 : j / 2);
                } else {
                    j -= (j / 2 < 1 ? 1 : j / 2);
                }
            }
            if (data->entities[j].id == current_entity_id) { // If entity was found successfully, update it
                commands[i] = current_entity.update_self(&current_room, &current_entity);
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

int main() {
    return 0;
}
