#ifndef BULLET_H
#define BULLET_H

#include "vector.h"
#include <SDL2/SDL.h>

typedef struct{
    vec2f position;
    vec2f velocity;
    vec2f initial_position;
}bullet_t;


void bullet_init(vec2f* pos, float angle);
void bullet_draw(SDL_Renderer* r);
void bullet_update(void);

#endif
