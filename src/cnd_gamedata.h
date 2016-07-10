/*
 * File: cnd_gamedata.h
 *
 * Header for all game data function declarations.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#ifndef CND_GAMEDATA_H
#define CND_GAMEDATA_H

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

void gamedata_free(t_game_data *);

int loop_update(t_game_data *);
int loop_render(t_game_data *);

#endif //CND_GAMEDATA_H
