/*
 * File: sprites.c
 *
 * Contains all methods for sprites.
 *
 * Author: Jack Romo <sharrackor@gmail.com>
 */

#include "cnoodle.h"
#include <stdlib.h>

t_sprite make_sprite(int period, int num_imgs, GLuint *texture) {
    t_sprite sprite;
    sprite.spr_id = 0;
    sprite.period = period;
    sprite.num_imgs = num_imgs;
    sprite.texture = texture;
    return sprite;
}

void free_sprite(t_sprite *sprite) {
    free(sprite->texture);
    free(sprite);
}

void draw_sprite(t_sprite *sprite /* add args needed when rendering finished */) {
    // TODO
}
