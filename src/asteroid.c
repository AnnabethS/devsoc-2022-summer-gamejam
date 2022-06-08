#include "asteroid.h"
#include "anna-layer.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <stdlib.h>
#include <time.h>
#include "gfx.h"
#include "player.h"
#include "vector.h"

#define STARTING_OFFSET 50
#define MAX_ASTEROIDS 40
#define ASTEROID_SPAWN_DELAY 0.2

#define ROCK_TEXTURES_COUNT 4
#define PICKUP_TEXTURES_COUNT 4


static asteroid* asteroids[MAX_ASTEROIDS] = {0};
static uint count;

SDL_Texture* rock_textures[ROCK_TEXTURES_COUNT];
SDL_Texture* pickup_textures[PICKUP_TEXTURES_COUNT];

local asteroid* asteroid_init()
{
    asteroid* a = malloc(sizeof(asteroid));

    int side = rand() % 3;

    vec2f target_pos;
    
    switch(side)
    {
    case 0: //top
        a->position.x = rand() % SCREENWIDTH;
        a->position.y = SCREENHEIGHT + STARTING_OFFSET;
        target_pos.x = rand() % SCREENWIDTH;
        target_pos.y = rand() % SCREENHEIGHT;
        break;
    case 1: // bot
        a->position.x = rand() % SCREENWIDTH;
        a->position.y = 0 - STARTING_OFFSET;
        target_pos.x = rand() % SCREENWIDTH;
        target_pos.y = rand() % SCREENHEIGHT;
        break;
    case 2: // left
        a->position.x = 0 - STARTING_OFFSET;
        a->position.y = rand() % SCREENHEIGHT;
        target_pos.x = rand() % SCREENWIDTH;
        target_pos.y = rand() % SCREENHEIGHT;
        break;
    case 3: // right
        a->position.x = SCREENWIDTH + STARTING_OFFSET;
        a->position.y = rand() % SCREENHEIGHT;
        target_pos.x = rand() % SCREENWIDTH;
        target_pos.y = rand() % SCREENHEIGHT;
        break;
    }

    a->velocity.x = 0;
    a->velocity.y = 0;

    vec2fDiff(&a->velocity, &a->position, &target_pos);
    vec2fClampMagnitude(&a->velocity, &a->velocity, 1 + (rand() % 10));

    a->rotational_velocity = (rand() % 6) + 1;
    a->rotation = rand() % 360;

    a->is_pickup = rand() % 2;

    if(a->is_pickup)
        a->texture = pickup_textures[rand() % PICKUP_TEXTURES_COUNT];
    else
        a->texture = rock_textures[rand() % ROCK_TEXTURES_COUNT];

    a->collision_radius = 24;
    a->render_rect.w = a->collision_radius * 1.5;
    a->render_rect.h = a->collision_radius * 1.5;

    a->on_screen = 1;
    
    return a;
}

local void asteroid_update(asteroid* a, player_t* p)
{
    vec2fAdd(&a->position, &a->position, &a->velocity);
    a->rotation += a->rotational_velocity;
    a->render_rect.x = (int)a->position.x - (a->render_rect.w / 2);
    a->render_rect.y = (int)a->position.y - (a->render_rect.h / 2);

    int low_x = -STARTING_OFFSET;
    int high_x = SCREENWIDTH + STARTING_OFFSET;
    int low_y = -STARTING_OFFSET;
    int high_y = SCREENHEIGHT + STARTING_OFFSET;
    
    if(a->on_screen)
    {
        if(a->position.x < low_x || a->position.x > high_x ||
           a->position.y < low_y || a->position.y > high_y)
        {
            asteroid_remove(a);
        }
    }
    else
    {
        if(a->position.x > 0 && a->position.x < SCREENWIDTH &&
           a->position.y > 0 && a->position.y < SCREENHEIGHT)
        {
            a->on_screen = 1;
        }
    }

    float big_rad = p->collision_radius;
    if(a->collision_radius > big_rad)
        big_rad = a->collision_radius;

    if(vec2fDist(&a->position, &p->position) <= big_rad)
    {
        asteroid_remove(a);
    }

}

local void asteroid_draw(SDL_Renderer* r, asteroid* a)
{
    SDL_RenderCopyEx(r, a->texture, NULL, &a->render_rect, a->rotation,
                     NULL, SDL_FLIP_NONE);
}

void asteroid_load_textures(SDL_Renderer* r)
{
    char path[20] = {0};
    for(int i=0; i < ROCK_TEXTURES_COUNT; i++)
    {
        sprintf(path, "res/rock%d.png", i+1);
        SDL_Surface* s = IMG_Load(path);
        rock_textures[i] = SDL_CreateTextureFromSurface(r, s);
        SDL_FreeSurface(s);
    }

    for(int i=0; i < PICKUP_TEXTURES_COUNT; i++)
    {
        sprintf(path, "res/pickup%d.png", i+1);
        SDL_Surface* s = IMG_Load(path);
        pickup_textures[i] = SDL_CreateTextureFromSurface(r, s);
        SDL_FreeSurface(s);
    }
}

void asteroid_add()
{
    if(count >= MAX_ASTEROIDS)
    {
        printf("ERROR: trying to add too many asteroids");
        return;
    }
    for(int i=0; i < MAX_ASTEROIDS; i++)
    {
        if(asteroids[i] == NULL)
        {
            asteroids[i] = asteroid_init();
            break;
        }
    }
    count++;
}

void asteroid_remove(asteroid* a)
{ //TODO
    free(a);
    for(int i=0; i < MAX_ASTEROIDS; i++)
    {
        if (a == asteroids[i])
        {
            asteroids[i] = NULL;
            break;
        }
    }
    count--;
}

void asteroid_all_update(player_t* p)
{ 
    persist long last_spawn = 0;
    if(count < MAX_ASTEROIDS)
    {
        if(time(NULL) - last_spawn > ASTEROID_SPAWN_DELAY)
        {
            last_spawn = time(NULL);
            asteroid_add();
        }
    }
    for(int i=0; i < MAX_ASTEROIDS; i++)
    {
        if(asteroids[i] != NULL)
        {
            asteroid_update(asteroids[i], p);
        }
    }
}

void asteroid_all_draw(SDL_Renderer* r)
{ 
    for(int i=0; i < MAX_ASTEROIDS; i++)
    {
        if(asteroids[i] != NULL)
        {
            asteroid_draw(r, asteroids[i]);
        }
    }
}

void asteroid_all_free()
{ //TODO
    
}
