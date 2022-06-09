#include "scrapyard.h"
#include "gamescreen.h"
#include "gfx.h"
#include "player.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>

#define SCRAPYARD_COUNT 4

#define SCRAPYARD_OPEN_TIME 20
#define SCRAPYARD_CLOSED_TIME 30

// N.B this is for each stage of the closing
#define SCRAPYARD_CLOSING_TIME 1


static SDL_Texture *scrapyard_open_texture;
static SDL_Texture *scrapyard_closing_textures[3];
static SDL_Texture *scrapyard_closed_texture;

struct scrapyard_t scrapyards[SCRAPYARD_COUNT];

local void check_player(struct scrapyard_t* s, player_t* p)
{
    if(p->position.x > s->rect.x &&
        p->position.x < s->rect.x + s->rect.w &&
        p->position.y > s->rect.y &&
        p->position.y < s->rect.y + s->rect.h &&
        p->weight != 0)
    {
        game_add_score(p->weight);
        p->weight = 0;
        game_change_weight(0);
        Mix_PlayChannel(-1, snd_dropoff, 0);
    }
}

void scrapyards_init(SDL_Renderer* r)
{
    SDL_Surface* s = IMG_Load("res/scrapyard.png");
    scrapyard_open_texture = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    s = IMG_Load("res/scrapyard-closed.png");
    scrapyard_closed_texture = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    char path[50] = {0};
    for(int i=0; i < 3; i++)
    {
        sprintf(path, "res/scrapyard-closing%d.png", i+1);
        s = IMG_Load(path);
        scrapyard_closing_textures[i] = SDL_CreateTextureFromSurface(r, s);
        SDL_FreeSurface(s);
    }

    srand(time(NULL));
    for(int i=0; i < SCRAPYARD_COUNT; i++)
    {
        scrapyards[i].state = rand() % 3;
        printf("%d\n", scrapyards[i].state);
        scrapyards[i].last_state_change = time(NULL)+5;
        scrapyards[i].rect.w = 128;
        scrapyards[i].rect.h = 128;
    }

    scrapyards[0].rect.x = 0;
    scrapyards[0].rect.y = 0;
    scrapyards[1].rect.x = SCREENWIDTH - 128;
    scrapyards[1].rect.y = 0;
    scrapyards[2].rect.x = 0;
    scrapyards[2].rect.y = SCREENHEIGHT - 128;
    scrapyards[3].rect.x = SCREENWIDTH - 128;
    scrapyards[3].rect.y = SCREENHEIGHT - 128;
}

void scrapyards_update(player_t* p)
{
    for(int i=0; i < SCRAPYARD_COUNT; i++)
    {
        switch(scrapyards[i].state)
        {
        case SCRAPYARD_OPEN:
            if((time(NULL) - scrapyards[i].last_state_change) > SCRAPYARD_OPEN_TIME)
            {
                scrapyards[i].state = SCRAPYARD_CLOSING;
                scrapyards[i].last_state_change = time(NULL);
            }
            else
            {
                check_player(&scrapyards[i], p);
            }
            break;
        case SCRAPYARD_CLOSING:
            if((time(NULL) - scrapyards[i].last_state_change) > SCRAPYARD_CLOSING_TIME*3)
            {
                scrapyards[i].state = SCRAPYARD_CLOSED;
                scrapyards[i].last_state_change = time(NULL);
            }
            else
            {
                check_player(&scrapyards[i], p);
            }
            break;
        case SCRAPYARD_CLOSED:
            if((time(NULL) - scrapyards[i].last_state_change) > SCRAPYARD_CLOSED_TIME)
            {
                scrapyards[i].state = SCRAPYARD_OPEN;
                scrapyards[i].last_state_change = time(NULL);
            }
            break;
        }
    }
}

void scrapyards_draw(SDL_Renderer* r)
{
    for(int i=0; i < SCRAPYARD_COUNT; i++)
    {
        SDL_Texture* draw_texture;
        switch(scrapyards[i].state)
        {
        case SCRAPYARD_OPEN:
            draw_texture = scrapyard_open_texture;
            break;
        case SCRAPYARD_CLOSED:
            draw_texture = scrapyard_closed_texture;
            break;
        case SCRAPYARD_CLOSING:
            {
                long delta_t = time(NULL) - scrapyards[i].last_state_change;
                if(delta_t < SCRAPYARD_CLOSING_TIME)
                    draw_texture = scrapyard_closing_textures[0];
                else if(delta_t < SCRAPYARD_CLOSING_TIME*2)
                    draw_texture = scrapyard_closing_textures[1];
                else
                    draw_texture = scrapyard_closing_textures[2];
            }
            break;
        }
        SDL_RenderCopy(r, draw_texture, NULL, &scrapyards[i].rect);
    }
}
