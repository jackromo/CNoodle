/*
 * File: sounds.c
 *
 * Contains all methods for sounds.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#include "cnoodle.h"
#include <stdlib.h>
#include <portaudio.h>

t_sound make_sound(char *snd_path, int volume) {
    t_sound sound;
    sound.snd_id = 0;
    sound.snd_path = snd_path;
    sound.volume = volume;
    return sound;
}

void free_sound(t_sound *sound) {
    free(sound->snd_path);
    free(sound);
}

void play_sound(t_sound *sound) {
    // TODO
}

void pause_sound(t_sound *sound) {
    //
}

void stop_sound(t_sound *sound) {
    //
}
