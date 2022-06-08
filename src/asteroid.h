#ifndef ASTEROID_H
#define ASTEROID_H

#include <SDL2/SDL_image.h>
#include "vector.h"
#include "player.h"

typedef struct asteroid_t{
    char is_pickup;
    float weight;
    vec2f position;
    vec2f velocity;
    float rotation;
    float rotational_velocity;
    SDL_Texture* texture;
    SDL_Rect render_rect;
    float collision_radius;
    char on_screen;
    struct asteroid_t* next;
    struct asteroid_t* prev;
}asteroid;

void asteroid_load_textures(SDL_Renderer* r);
void asteroid_add();
void asteroid_remove(asteroid* a);
void asteroid_all_update(player_t* p);
void asteroid_all_draw(SDL_Renderer* r);
void asteroid_all_free();

#endif
