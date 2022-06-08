#ifndef PLAYER_H
#define PLAYER_H

#include "vector.h"
#include "anna-layer.h"
#include <SDL2/SDL_image.h>

typedef enum {THRUST_ACCELERATE, THRUST_NONE, THRUST_REVERSE} thrust_state;
typedef enum {TURN_LEFT, TURN_RIGHT, TURN_NONE} turn_state;

typedef struct{
    vec2f position;
    vec2f velocity;
    float max_acceleration;
    float max_speed;
    float rotation;
    float max_turn_speed;
    float weight;
    float max_weight;
    SDL_Texture* texture;
    SDL_Rect render_rect;
}player_t;

void player_init(player_t *p, SDL_Renderer *r);
void player_update(player_t* p, thrust_state thrust, turn_state turn);
void player_draw(player_t* p, SDL_Renderer* r);

#endif
