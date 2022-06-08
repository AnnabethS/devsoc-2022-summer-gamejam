#include "player.h"
#include "vector.h"
#include "sdl_util.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>

local float weight_scale_factor(player_t* p)
{
    float scale = ((-1 * (p->weight / p->max_weight)) + 1);
    if (scale < 0.1)
        scale = 0.1;
    return scale;
}

void player_init(player_t* p, SDL_Renderer* r)
{
    memset(p, 0, sizeof(player_t));
    SDL_Surface* s = IMG_Load("res/player.png");
    if(s == NULL)
    {
        fprintf(stderr, "could not load player texture\n");
        return;
    }
    p->texture = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);
    p->render_rect.w = 64;
    p->render_rect.h = 64;
    p->collision_radius = 32;

    p->position.x = 100;
    p->position.y = 100;

    p->max_acceleration = 0.5;
    p->max_speed = 6;
    p->max_turn_speed = 5;
    p->max_weight = 10;
    p->weight = 0;
}

void player_update(player_t* p, thrust_state thrust, turn_state turn)
{
    /* p->render_rect.w = 64; */
    /* p->render_rect.h = 64; */
    /* p->collision_radius = 48; */
    /* // turn */

    float turn_amount = weight_scale_factor(p) * p->max_turn_speed;
    
    switch(turn)
    {
    case TURN_LEFT:
        turn_amount *= -1;
        break;
    case TURN_RIGHT:
        break;
    case TURN_NONE:
        turn_amount = 0;
        break;
    }
    p->rotation += turn_amount;
    
    // accelerate
    float acceleration_scale = weight_scale_factor(p) * p->max_acceleration;
    vec2f change_vel;
    change_vel.x = 0;
    change_vel.y = 0;
    switch (thrust) {
    case THRUST_ACCELERATE:
        vec2fNormalisedVectorFromAngleDegrees(&change_vel, p->rotation);
        vec2fScalarProduct(&change_vel, &change_vel, acceleration_scale);
        break;
    case THRUST_REVERSE:
        vec2fNormalisedVectorFromAngleDegrees(&change_vel, p->rotation+180);
        vec2fScalarProduct(&change_vel, &change_vel, acceleration_scale);
        break;
    case THRUST_NONE:
        if(vec2fMagnitude(&p->velocity) > acceleration_scale)
        {
            vec2fNormalise(&change_vel, &p->velocity);
            vec2fScalarProduct(&change_vel, &change_vel, -acceleration_scale);
        }
        else if (vec2fMagnitude(&p->velocity) > 0)
        {
            p->velocity.x = 0;
            p->velocity.y = 0;
        }
    }

    vec2fAdd(&p->velocity, &p->velocity, &change_vel);

    float scaled_max_speed = weight_scale_factor(p) * p->max_speed;

    vec2fClampMagnitude(&p->velocity, &p->velocity, scaled_max_speed);
    // do move

    vec2fAdd(&p->position, &p->position, &p->velocity);

    // recalculate render position
    p->render_rect.x = ((int)p->position.x) - (p->render_rect.w/2);
    p->render_rect.y = ((int)p->position.y) - (p->render_rect.h/2);
}

void player_draw(player_t* p, SDL_Renderer* r)
{
    SDL_RenderCopyEx(r, p->texture, NULL, &p->render_rect, p->rotation+90,
                     NULL, SDL_FLIP_NONE);
}
