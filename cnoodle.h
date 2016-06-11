/*
 * File: cnoodle.h
 *
 * Main header in CNoodle.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#ifndef CNOODLE_H
#define CNOODLE_H

struct game_data;
struct entity;
struct room;
struct sprite;
struct sound;
struct update_command;
struct update_command_container;

// TODO: remove t_
typedef struct game_data t_game_data;
typedef struct entity t_entity;
typedef struct room t_room;
typedef struct sprite t_sprite;
typedef struct sound t_sound;
typedef struct update_command t_update_command;
typedef struct update_command_container t_update_command_container;

int update(t_game_data*);
int render(t_game_data*);

#endif //CNOODLE_H
