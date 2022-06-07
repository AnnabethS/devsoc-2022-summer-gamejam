#include "player.h"
#include "vector.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>

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
    //SDL_QueryTexture(p->texture, NULL, NULL, &p->render_rect.w, &p->render_rect.h);
    p->render_rect.w = 64;
    p->render_rect.h = 64;

    p->position.x = 100;
    p->position.y = 100;
}

void player_update(player_t* p, thrust_state thrust, turn_state turn)
{
    // turn
    // TODO: make this not use a constant

    float turn_amount;
    
    switch(turn)
    {
    case TURN_LEFT:
        turn_amount = -5;
        break;
    case TURN_RIGHT:
        turn_amount = 5;
        break;
    case TURN_NONE:
        turn_amount = 0;
        break;
    }
    p->rotation += turn_amount;
    
    // accelerate
    vec2f change_vel;
    change_vel.x = 0;
    change_vel.y = 0;
    switch (thrust) {
    case THRUST_ACCELERATE:
        vec2fNormalisedVectorFromAngleDegrees(&change_vel, p->rotation);
        vec2fScalarProduct(&change_vel, &change_vel, 0.5);
        break;
    case THRUST_REVERSE:
        vec2fNormalisedVectorFromAngleDegrees(&change_vel, p->rotation+180);
        vec2fScalarProduct(&change_vel, &change_vel, 0.5);
        break;
    case THRUST_NONE:
        if(vec2fMagnitude(&p->velocity) > 0.5)
        {
            vec2fNormalise(&change_vel, &p->velocity);
            vec2fScalarProduct(&change_vel, &change_vel, -0.5);
        }
        else if (vec2fMagnitude(&p->velocity) > 0)
        {
            p->velocity.x = 0;
            p->velocity.y = 0;
        }
        break;
    }

    vec2fAdd(&p->velocity, &p->velocity, &change_vel);
    vec2fClampMagnitude(&p->velocity, &p->velocity, 6);

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
